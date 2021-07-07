#pragma once

#include <Audilets/DSP/Math.h>

namespace audilets::dsp::filters
{
  class ChamberlinFilter
  {
  public:

    enum class Type { LP, HP, BP };

    ChamberlinFilter(const size_t frameSampleRate, const float filterFrequency, const ChamberlinFilter::Type filterType);
    ~ChamberlinFilter();

    float frequency() const;
    void frequency(float value);

    ChamberlinFilter::Type type() const;
    void type(const ChamberlinFilter::Type value);

    float get(const ChamberlinFilter::Type value) const;
    float get() const;
    float set(float value);

  private:

    const size_t frameSampleRate;
    float filterFrequency;
    ChamberlinFilter::Type filterType;
    float f, q;
    float lp0, hp0, bp0;
  };
}
