#include <Audilets/IO/AudioSource.h>

using namespace audilets::io;

AudioSource::AudioSource(const std::string& device_name, size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size) :
  Source(frame_sample_rate, frame_size, frame_buffer_size),
  audio_device_name(device_name),
  audio_device_id(std::nullopt),
  audio_frame_buffer(
    frame_buffer_size,
    [frame_size](size_t index)
    {
      auto input = new InputFrame();
      input->index = index;
      input->frame.resize(frame_size);
      input->timestamp = 0;
      input->status = 0;
      return input;
    },
    [](InputFrame* input)
    {
      delete input;
    })
{
}

void AudioSource::open()
{
  if (audio.isStreamRunning())
  {
    audio.stopStream();
  }

  if (audio.isStreamOpen())
  {
    audio.closeStream();
  }

  audio_device_id.reset();

  if (audio_device_name.empty())
  {
    audio_device_id = audio.getDefaultInputDevice();
  }
  else
  {
    const std::regex device_name_regex(
      ".*" + audio_device_name + ".*",
      std::regex_constants::icase);

    const size_t devices = audio.getDeviceCount();

    for (size_t i = 0; i < devices; ++i)
    {
      const RtAudio::DeviceInfo device = audio.getDeviceInfo(i);

      if (!device.probed)
      {
        continue;
      }

      if (device.inputChannels < 1)
      {
        continue;
      }

      if (!std::regex_match(device.name, device_name_regex))
      {
        continue;
      }

      audio_device_id = i;
      break;
    }
  }

  if (!audio_device_id)
  {
    throw std::runtime_error("Audio source " + audio_device_name + " not found!");
  }

  RtAudio::StreamParameters parameters;
  parameters.deviceId = audio_device_id.value();
  parameters.nChannels = 1;
  parameters.firstChannel = 0;

  uint32_t new_frame_size = frame_size();

  audio.openStream(
    nullptr,
    &parameters,
    RTAUDIO_FLOAT32,
    frame_sample_rate(),
    &new_frame_size,
    &AudioSource::callback,
    this);

  if (new_frame_size != frame_size())
  {
    throw std::runtime_error("Unexpected audio source stream frame size " + std::to_string(new_frame_size) + "!");
  }
}

void AudioSource::close()
{
  if (audio.isStreamRunning())
  {
    audio.stopStream();
  }

  if (audio.isStreamOpen())
  {
    audio.closeStream();
  }
}

void AudioSource::start()
{
  if (!audio.isStreamOpen())
  {
    return;
  }

  if (audio.isStreamRunning())
  {
    audio.stopStream();
  }

  audio.startStream();
}

void AudioSource::stop()
{
  if (!audio.isStreamOpen())
  {
    return;
  }

  if (!audio.isStreamRunning())
  {
    return;
  }

  audio.stopStream();
}

void AudioSource::read(std::function<void(const std::vector<float>& frame)> callback)
{
  const auto ok = audio_frame_buffer.read(frame_sample_period(), [&](InputFrame& input)
  {
    callback(input.frame);
  });

  if (!ok)
  {
    std::cout << "UNABLE TO READ FROM INPUT FRAME FIFO!" << std::endl;
  }
}

int AudioSource::callback(void* output_frame_data, void* input_frame_data, uint32_t frame_size, double timestamp, RtAudioStreamStatus status, void* $this)
{
  auto& audio_frame_buffer = static_cast<AudioSource*>($this)->audio_frame_buffer;

  const auto ok = audio_frame_buffer.write([&](InputFrame& input)
  {
    if (input.frame.size() != frame_size)
    {
      std::cout << "UNEXPECTED INPUT FRAME SIZE!" << std::endl;
    }

    std::memcpy(
      input.frame.data(), input_frame_data,
      std::min(input.frame.size(), (size_t)frame_size) *
        sizeof(input.frame.front()));
  });

  if (!ok)
  {
    std::cout << "UNABLE TO WRITE TO INPUT FRAME FIFO!" << std::endl;
  }

  if (status != 0)
  {
    std::cout << "AUDIO SOURCE STREAM STATUS " << status << std::endl;
  }

  return 0;
}
