#pragma once

#include "FFT.h"
#include "Math.h"
#include "Window.h"

#include <vector>

namespace Audilets::DSP
{
  class STFT
  {
  public:

    STFT(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate);
    ~STFT();

    void stft(const short* frame, short* reframe);

  protected:

    const size_t frameSampleRate;
    const size_t frameSize;
    const size_t frameHalfSize;
    const size_t frameHopRate;
    const size_t frameHopSize;

    virtual void processInTimeDomain(float* frame) {};
    virtual void processInFrequencyDomain(std::complex<float>* frame) {};

  private:

    short* analysisBuffer;
    short* synthesisBuffer;
    float* frameBuffer;
    float* frameWindow;
    std::vector<size_t> frameHops;

    FFT* fft;
  };
}
