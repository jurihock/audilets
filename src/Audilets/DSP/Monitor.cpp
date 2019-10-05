#include "Monitor.h"

using namespace Audilets::DSP;

Monitor::Monitor(const size_t frameSampleRate, const size_t frameSize) :
  frameSampleRate(frameSampleRate),
  frameSize(frameSize),
  frameHalfSize(frameSize / 2)
{
  assert(frameSampleRate > 0);
  assert(frameSize > 0);

  frameBuffer = new float[frameSize]();
  frameWindow = new float[frameSize]();

  Window(frameSize, /*hops*/1, /*periodic*/true, /*weighted*/true).hann(frameWindow);

  fft = new FFT(frameSize, /*shift*/true, /*normalize*/true);
  vocoder = new BernseeVocoder(frameSampleRate, frameSize, /*hops*/1);
}

Monitor::~Monitor()
{
  if (frameBuffer != nullptr)
  {
    delete[] frameBuffer;
    frameBuffer = nullptr;
  }

  if (frameWindow != nullptr)
  {
    delete[] frameWindow;
    frameWindow = nullptr;
  }

  if (fft != nullptr)
  {
    delete fft;
    fft = nullptr;
  }

  if (vocoder != nullptr)
  {
    delete vocoder;
    vocoder = nullptr;
  }
}

void Monitor::magnitudes(const short* frame, std::vector<float>& magnitudes)
{
  Codec::short2float(frame, frameBuffer, frameWindow, frameSize);

  fft->fft(frameBuffer);

  auto frameBuffer2 = reinterpret_cast<std::complex<float>*>(frameBuffer);

  vocoder->analyze(frameBuffer2);

  magnitudes.clear();
  magnitudes.resize(frameHalfSize);

  for (size_t i = 0; i < frameHalfSize; ++i)
  {
    magnitudes[i] = frameBuffer2[i].real();
  }
}

void Monitor::frequencies(const short* frame, std::vector<float>& frequencies)
{
  Codec::short2float(frame, frameBuffer, frameWindow, frameSize);

  fft->fft(frameBuffer);

  auto frameBuffer2 = reinterpret_cast<std::complex<float>*>(frameBuffer);

  vocoder->analyze(frameBuffer2);

  frequencies.clear();
  frequencies.resize(frameHalfSize);

  for (size_t i = 0; i < frameHalfSize; ++i)
  {
    frequencies[i] = frameBuffer2[i].imag();
  }
}