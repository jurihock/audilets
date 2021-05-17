#pragma once

#include <Audilets/DSP/FIFO.h>
#include <Audilets/IO/Sink.h>

#include <rtaudio/RtAudio.h>

#include <optional>
#include <regex>
#include <string>
#include <vector>

using audilets::dsp::FIFO;

namespace audilets::io
{
  class AudioSink : public Sink<float>
  {
  public:

    AudioSink(const std::string& device_name, size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size);

    const std::string& device_name() const { return audio_device_name; }
    std::optional<size_t> device_id() const { return audio_device_id; }

    void open() override;
    void close() override;

    void start() override;
    void stop() override;

    void write(const std::vector<float>& frame) override;

  private:

    struct OutputFrame
    {
      size_t index;
      std::vector<float> frame;
    };

    const std::string audio_device_name;
    std::optional<size_t> audio_device_id;
    FIFO<OutputFrame> audio_frame_buffer;
    RtAudio audio;

    static int callback(void* output_frame_data, void* input_frame_data, uint32_t frame_size, double timestamp, RtAudioStreamStatus status, void* $this);
  };
}
