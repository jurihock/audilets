#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iterator>
#include <limits>
#include <numeric>

namespace audilets::dsp::math
{
  template<typename T, uint8_t multiplier = 1>
  inline constexpr T const PI = T(multiplier) * T(M_PI); // std::acos(-T(1))

  template<typename T, uint8_t multiplier = 1>
  inline constexpr const T IP = T(1) / PI<T, multiplier>;

  /**
   * Wraps an arbitrary radian phase value into the range [-PI,+PI).
   **/
  template<typename T>
  inline T wrap(const T phase)
  {
    return phase - math::PI<T, 2> * std::floor(phase * math::IP<T, 2> + T(0.5));
  }

  /**
   * Computes a * (1-c) + b * c, where c is in range [0,1].
   **/
  template<typename T>
  inline T lerp(const T a, const T b, const T c)
  {
    assert(0 <= c && c <= 1);
    return a + c * (b - a);
  }

  template<typename T>
  inline const T& clamp(const T& value, const T& lo, const T& hi)
  {
    assert(lo <= hi);
    return (value < lo) ? lo : (hi < value) ? hi : value;
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  inline T mean(const IT begin, const IT end)
  {
    double mean = 0.0;
    size_t count = 0;

    for (auto it = begin; it != end; ++it)
    {
      mean += *it;
      ++count;
    }

    mean /= count;

    return static_cast<T>(mean);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  inline T stdev(const IT begin, const IT end, const T mean)
  {
    double stdev = 0.0;
    size_t count = 0;

    for (auto it = begin; it != end; ++it)
    {
      stdev += (*it - static_cast<double>(mean)) * (*it - static_cast<double>(mean));
      ++count;
    }

    stdev /= count - 1;
    stdev = std::sqrt(stdev);

    return static_cast<T>(stdev);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  inline T stdev(const IT begin, const IT end)
  {
    return stdev(begin, end, mean(begin, end));
  }

  template<class T>
  inline T decibel(const T value)
  {
    return T(10) * std::log10(value);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  inline void decibel(const IT begin, const IT end)
  {
    for (auto it = begin; it != end; ++it)
    {
      *it = decibel(*it);
    }
  }
}
