#pragma once

#include "Math.h"

namespace Audilets::DSP
{
  class SineWave
  {
  public:

    SineWave(const size_t sampleRate, const size_t waveFrequency);
    ~SineWave();

    size_t getSampleRate() const;
    void setSampleRate(const size_t sampleRate);

    size_t getWaveFrequency() const;
    void setWaveFrequency(const size_t waveFrequency);

    float getNextPhase();
    float getNextValue();

  private:

    size_t sampleRate;
    size_t waveFrequency;

    float phaseDeviation;
    float phaseDivisor;

  public:

    template<typename T>
    T getNextValue(const T gain)
    {
      return static_cast<T>(gain * getNextValue());
    }

    template<typename T>
    void getNextValue(T* values, const size_t count)
    {
      for (size_t i = 0; i < count; ++i)
      {
        values[i] = static_cast<T>(getNextValue());
      }
    }

    template<typename T>
    void getNextValue(T* values, const size_t count, const T gain)
    {
      for (size_t i = 0; i < count; ++i)
      {
        values[i] = static_cast<T>(gain * getNextValue());
      }
    }
  };
}
