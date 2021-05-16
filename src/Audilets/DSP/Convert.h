#pragma once

#include <Audilets/DSP/Math.h>

namespace audilets::dsp::convert
{
  inline float to_float(short value)
  {
    return (static_cast<float>(value) + 0.5f) / 32767.5f;
  }

  inline short to_short(float value)
  {
    return static_cast<short>((value * 32767.5f) - 0.5f);
  }

  inline void from_short_to_float(const short* source, float* destination, const float* window, const size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      destination[i] = math::clamp(to_float(source[i]) * window[i], -1.0f, +1.0f);
    }
  }

  inline void from_float_to_short(const float* source, short* destination, const float* window, const size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      destination[i] = to_short(math::clamp(source[i] * window[i], -1.0f, +1.0f));
    }
  }

  inline void from_float_to_float(const float* source, float* destination, const float* window, const size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      destination[i] = math::clamp(source[i] * window[i], -1.0f, +1.0f);
    }
  }

  inline void from_float_to_short_plus(const float* source, short* destination, const float* window, const size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      destination[i] += /* plus */ to_short(math::clamp(source[i] * window[i], -1.0f, +1.0f));
    }
  }

  inline void from_float_to_float_plus(const float* source, float* destination, const float* window, const size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      destination[i] += /* plus */ math::clamp(source[i] * window[i], -1.0f, +1.0f);
    }
  }
}
