#pragma once

#include <Audilets/DSP/Math.h>

namespace audilets::dsp::filters
{
  class ChamberlinFilter
  {
  public:

    enum class Type { Bypass, Lowpass, Highpass, Bandpass, Bandstop };

    ChamberlinFilter(const size_t frameSampleRate, const float filterFrequency, const ChamberlinFilter::Type filterType);
    ~ChamberlinFilter();

    float frequency() const;
    void frequency(float filterFrequency);

    ChamberlinFilter::Type type() const;
    void type(const ChamberlinFilter::Type filterType);

    float get(const ChamberlinFilter::Type filterType) const;
    float get() const;
    float set(float value);

    float operator()(float value) { return set(value); }

  private:

    const size_t frameSampleRate;
    float filterFrequency;
    ChamberlinFilter::Type filterType;
    float f, q;
    float value, lowpass, highpass, bandpass, bandstop;
  };
}
