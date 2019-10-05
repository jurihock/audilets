#pragma once

#include "Algorithms/BernseeVocoder.h"
#include "Convert.h"
#include "FFT.h"
#include "Math.h"
#include "Window.h"

#include <vector>

using Audilets::DSP::Algorithms::BernseeVocoder;

namespace Audilets::DSP
{
  class Monitor
  {
  public:

    Monitor(const size_t frameSampleRate, const size_t frameSize);
    ~Monitor();

    void magnitudes(const short* frame, std::vector<float>& magnitudes);
    void frequencies(const short* frame, std::vector<float>& frequencies);

  protected:

    const size_t frameSampleRate;
    const size_t frameSize;
    const size_t frameHalfSize;

  private:

    float* frameBuffer;
    float* frameWindow;

    FFT* fft;
    BernseeVocoder* vocoder;
  };
}
