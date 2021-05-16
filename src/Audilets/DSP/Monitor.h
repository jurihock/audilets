#pragma once

#include <Audilets/DSP/Algorithms/BernseeVocoder.h>
#include <Audilets/DSP/Convert.h>
#include <Audilets/DSP/FFT.h>
#include <Audilets/DSP/Math.h>
#include <Audilets/DSP/Window.h>

#include <vector>

using audilets::dsp::algorithms::BernseeVocoder;

namespace audilets::dsp
{
  class Monitor
  {
  public:

    Monitor(const size_t frameSampleRate, const size_t frameSize);
    ~Monitor();

    void magnitudes(const short* frame, std::vector<float>& magnitudes);
    void frequencies(const short* frame, std::vector<float>& frequencies);
    void milliseconds(std::vector<float>& milliseconds);

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
