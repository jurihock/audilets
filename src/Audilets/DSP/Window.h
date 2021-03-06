#pragma once

#include <Audilets/DSP/Math.h>

namespace audilets::dsp
{
  class Window
  {
  public:

    Window(const size_t frameSize, const size_t frameHopRate, const bool periodic, const bool weighted);
    ~Window();

    void hann(float* window) const;

  private:

    const size_t frameSize;
    const size_t frameHopRate;
    const size_t frameHopSize;
    const bool periodic;
    const bool weighted;

    void weight(float* window) const;
  };
}
