#pragma once

#include <Audilets/DSP/Convert.h>
#include <Audilets/DSP/FFT.h>
#include <Audilets/DSP/Math.h>
#include <Audilets/DSP/Window.h>

#include <vector>

namespace audilets::dsp
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
