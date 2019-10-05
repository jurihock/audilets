#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iterator>
#include <numeric>

namespace Audilets::DSP::Math
{
  template<typename T>
  constexpr T const PI = std::acos(-T(1));

  template<typename T>
  constexpr const T IP = T(1) / PI<T>;

  /**
   * Returns the principal value in range (-PI,+PI]
   * of the specified "unwrapped" phase angle.
   */
  template<typename T>
  constexpr T wrap(const T phase)
  {
    long tmp = phase * IP<T>;

    if (tmp < 0) tmp -= tmp & 1;
    else         tmp += tmp & 1;

    return phase - tmp * PI<T>;
  }

  template<class T>
  constexpr const T& clamp(const T& value, const T& lo, const T& hi)
  {
    assert(lo <= hi);
    return (value < lo) ? lo : (hi < value) ? hi : value;
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  constexpr T mean(const IT begin, const IT end)
  {
    double mean = 0.0;
    size_t count = 0;

    std::for_each(begin, end, [&](const T& value)
    {
      mean += value;
      ++count;
    });

    mean /= count;

    return static_cast<T>(mean);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  constexpr T stdev(const IT begin, const IT end, const T mean)
  {
    double stdev = 0.0;
    size_t count = 0;

    std::for_each(begin, end, [&](const T& value)
    {
      stdev += (value - static_cast<double>(mean)) * (value - static_cast<double>(mean));
      ++count;
    });

    stdev /= count - 1;
    stdev = std::sqrt(stdev);

    return static_cast<T>(stdev);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  constexpr T stdev(const IT begin, const IT end)
  {
    return stdev(begin, end, mean(begin, end));
  }

  template<class T>
  constexpr T decibel(const T value)
  {
    return T(10) * std::log10(value);
  }

  template<class IT, typename T = typename std::iterator_traits<IT>::value_type>
  constexpr void decibel(const IT begin, const IT end)
  {
    for (auto it = begin; it != end; ++it)
    {
      *it = decibel(*it);
    }
  }
}
