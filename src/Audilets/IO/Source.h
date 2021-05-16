#pragma once

#include <chrono>
#include <cmath>
#include <iostream>
#include <functional>

namespace audilets::io
{
  template<typename T>
  class Source
  {
  public:

    Source(size_t frame_sample_rate, size_t frame_size, size_t frame_buffer_size) :
        source_frame_sample_rate(frame_sample_rate),
        source_frame_size(frame_size),
        source_frame_buffer_size(frame_buffer_size),
        source_frame_sample_period(std::chrono::milliseconds(
          static_cast<std::chrono::milliseconds::rep>(
            std::ceil(1e3 * frame_size / frame_sample_rate)))) {}

    size_t frame_sample_rate() const { return source_frame_sample_rate; }
    size_t frame_size() const { return source_frame_size; }
    size_t frame_buffer_size() const { return source_frame_buffer_size; }
    const std::chrono::milliseconds& frame_sample_period() const { return source_frame_sample_period; }

    virtual ~Source() {}

    virtual void open() = 0;
    virtual void close() = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void read(std::function<void(const std::vector<T>& frame)> callback) = 0;

  private:

    const size_t source_frame_sample_rate;
    const size_t source_frame_size;
    const size_t source_frame_buffer_size;
    const std::chrono::milliseconds source_frame_sample_period;
  };
}
