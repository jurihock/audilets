#pragma once

#include "Math.h"

namespace audilets::dsp::convert
{
  inline void fromShortToFloat(const short* source, float* destination, const float* window, const size_t count)
  {
    const float scaleToFloat = 1.0 / 32767.5;
    const float scaleCorrection = 0.5;

    float value;

    for (size_t i = 0; i < count; ++i)
    {
      value = static_cast<float>(source[i]);
      value += scaleCorrection;
      value *= scaleToFloat;
      value *= window[i];

      destination[i] = value;
    }
  }

  inline void fromFloatToShort(const float* source, short* destination, const float* window, const size_t count)
  {
    const float scaleToShort = 32767.5;
    const float scaleCorrection = 0.5;

    float value;

    for (size_t i = 0; i < count; ++i)
    {
      value = math::clamp(source[i] * window[i], -1.0f, +1.0f);
      value *= scaleToShort;
      value -= scaleCorrection;

      destination[i] = static_cast<short>(value);
    }
  }

  inline void fromFloatToShortPlus(const float* source, short* destination, const float* window, const size_t count)
  {
    const float scaleToShort = 32767.5;
    const float scaleCorrection = 0.5;

    float value;

    for (size_t i = 0; i < count; ++i)
    {
      value = math::clamp(source[i] * window[i], -1.0f, +1.0f);
      value *= scaleToShort;
      value -= scaleCorrection;

      destination[i] /*plus*/ += static_cast<short>(value);
    }
  }
}
