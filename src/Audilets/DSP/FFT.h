#pragma once

#include "Math.h"

#include <kissfft/kiss_fftr.h>

namespace Audilets::DSP
{
  class FFT
  {
  public:

    FFT(const size_t frameSize, const bool shiftFrame, const bool normalizeFrame);
    ~FFT();

    void fft(float* frame) const;
    void ifft(float* frame) const;

  private:

    const size_t frameSize;
    const size_t frameHalfSize;
    const bool shiftFrame;
    const bool normalizeFrame;

    kiss_fftr_cfg forward;
    kiss_fftr_cfg backward;
    kiss_fft_cpx* buffer;

    void shift(float* frame) const;
    void normalize(float* frame) const;
  };
}
