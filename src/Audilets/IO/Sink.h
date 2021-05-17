#pragma once

#include <chrono>
#include <cmath>
#include <functional>

namespace audilets::io
{
  template<typename T>
  class Sink
  {
  public:

    Sink(size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size) :
      sink_frame_sample_rate(frame_sample_rate),
      sink_frame_size(frame_size),
      sink_frame_buffer_size(frame_buffer_size),
      sink_frame_sample_period(std::chrono::milliseconds(
        static_cast<std::chrono::milliseconds::rep>(
          std::ceil(1e3 * frame_size / frame_sample_rate)))) {}

    virtual ~Sink() {}

    size_t frame_sample_rate() const { return sink_frame_sample_rate; }
    size_t frame_size() const { return sink_frame_size; }
    size_t frame_buffer_size() const { return sink_frame_buffer_size; }
    const std::chrono::milliseconds& frame_sample_period() const { return sink_frame_sample_period; }

    virtual void open() = 0;
    virtual void close() = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void write(const std::vector<T>& frame) = 0;

  private:

    const size_t sink_frame_sample_rate;
    const size_t sink_frame_size;
    const size_t sink_frame_buffer_size;
    const std::chrono::milliseconds sink_frame_sample_period;
  };
}
