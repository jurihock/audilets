#include "Window.h"

#include <cassert>

using namespace Audilets::DSP;

Window::Window(const size_t frameSize, const size_t frameHopRate, const bool periodic, const bool weighted) :
  frameSize(frameSize),
  frameHopRate(frameHopRate),
  frameHopSize(frameSize / frameHopRate),
  periodic(periodic),
  weighted(weighted)
{
  assert(frameSize > 0);
  assert(frameHopRate > 0);
}

Window::~Window()
{
}

void Window::weight(float* window) const
{
  float weighting = 0.0f;

  for (size_t i = 0; i < frameSize; ++i)
  {
    weighting += window[i] * window[i];
  }

  weighting = 1.0f / std::sqrt(weighting / frameHopSize);

  for (size_t i = 0; i < frameSize; ++i)
  {
    window[i] *= weighting;
  }
}

void Window::hann(float* window) const
{
  const size_t n = (periodic) ? frameSize + 1 : frameSize;

  for (size_t i = 0; i < frameSize; ++i)
  {
    window[i] = 0.5f * (1.0f - std::cos(2.0f * Math::PI<float> * i / (n - 1.0f)));
  }

  if (weighted)
  {
    weight(window);
  }
}
