#pragma once

#include <Audilets/DSP/Math.h>
#include <Audilets/DSP/STFT.h>

#include <Audilets/DSP/Algorithms/BernseeVocoder.h>

namespace audilets::dsp::algorithms
{
  class SinglePitchShift : public STFT
  {
  public:

    SinglePitchShift(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate);
    ~SinglePitchShift();

    float getPitchShiftingFactor() const;
    void setPitchShiftingFactor(const float value);

  private:

    BernseeVocoder vocoder;

    float pitchShiftingFactor;

    std::complex<float>* frameBuffer;

    void processInTimeDomain(float* frame) override;
    void processInFrequencyDomain(std::complex<float>* frame) override;

    void process(std::complex<float>* frame);
  };
}
