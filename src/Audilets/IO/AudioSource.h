#pragma once

#include <Audilets/DSP/FIFO.h>
#include <Audilets/IO/Source.h>

#include <rtaudio/RtAudio.h>

#include <regex>
#include <string>
#include <vector>

using audilets::dsp::FIFO;

namespace audilets::io
{
  class AudioSource : public Source<float>
  {
  public:

    AudioSource(const std::string& device_name, size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size);

    const std::string& device_name() const { return audio_device_name; }
    size_t device_id() const { return audio_device_id; }

    void open() override;
    void close() override;

    void start() override;
    void stop() override;

    void read(std::function<void(const std::vector<float>& frame)> callback) override;

  private:

    struct InputFrame
    {
      size_t index;
      std::vector<float> frame;
      double timestamp;
      RtAudioStreamStatus status;
    };

    const std::string audio_device_name;
    size_t audio_device_id;
    FIFO<InputFrame> audio_frame_buffer;
    RtAudio audio;

    static int callback(void* output_frame_data, void* input_frame_data, uint32_t frame_size, double timestamp, RtAudioStreamStatus status, void* $this);
  };
}
