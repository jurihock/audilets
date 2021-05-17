#include <Audilets/IO/AudioSink.h>

using namespace audilets::io;

AudioSink::AudioSink(const std::string& device_name, size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size) :
  Sink(frame_sample_rate, frame_size, frame_buffer_size),
  audio_device_name(device_name),
  audio_device_id(std::nullopt),
  audio_frame_buffer(
    frame_buffer_size,
    [frame_size](size_t index)
    {
      auto output = new OutputFrame();
      output->index = index;
      output->frame.resize(frame_size);
      return output;
    },
    [](OutputFrame* output)
    {
      delete output;
    })
{
}

void AudioSink::open()
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

    if (device.outputChannels < 1)
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

  if (!audio_device_id)
  {
    throw std::runtime_error("Audio sink " + audio_device_name + " not found!");
  }

  RtAudio::StreamParameters parameters;
  parameters.deviceId = audio_device_id.value();
  parameters.nChannels = 1;
  parameters.firstChannel = 0;

  uint32_t new_frame_size = frame_size();

  audio.openStream(
    &parameters,
    nullptr,
    RTAUDIO_FLOAT32,
    frame_sample_rate(),
    &new_frame_size,
    &AudioSink::callback,
    this);

  if (new_frame_size != frame_size())
  {
    throw std::runtime_error("Unexpected audio sink stream frame size " + std::to_string(new_frame_size) + "!");
  }
}

void AudioSink::close()
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

void AudioSink::start()
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

void AudioSink::stop()
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

void AudioSink::write(const std::vector<float>& frame)
{
  const auto ok = audio_frame_buffer.write([&](OutputFrame& output)
  {
    output.frame = frame;
  });

  if (!ok)
  {
    std::cout << "UNABLE TO WRITE TO OUTPUT FRAME FIFO!" << std::endl;
  }
}

int AudioSink::callback(void* output_frame_data, void* input_frame_data, uint32_t frame_size, double timestamp, RtAudioStreamStatus status, void* $this)
{
  auto& audio_frame_buffer = static_cast<AudioSink*>($this)->audio_frame_buffer;

  const auto ok = audio_frame_buffer.read([&](OutputFrame& output)
  {
    if (output.frame.size() != frame_size)
    {
      std::cout << "UNEXPECTED OUTPUT FRAME SIZE!" << std::endl;
    }

    std::memcpy(
      output_frame_data, output.frame.data(),
      std::min(output.frame.size(), (size_t)frame_size) *
        sizeof(output.frame.front()));
  });

  if (!ok)
  {
    std::cout << "UNABLE TO READ FROM OUTPUT FRAME FIFO!" << std::endl;
  }

  if (status != 0)
  {
    std::cout << "AUDIO SINK STREAM STATUS " << status << std::endl;
  }

  return 0;
}
