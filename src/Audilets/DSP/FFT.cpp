#include "FFT.h"

using namespace Audilets::DSP;

FFT::FFT(const size_t frameSize, const bool shiftFrame, const bool normalizeFrame) :
  frameSize(frameSize),
  frameHalfSize(frameSize / 2),
  shiftFrame(shiftFrame),
  normalizeFrame(normalizeFrame)
{
  forward = kiss_fftr_alloc(frameSize, 0, nullptr, nullptr);
  backward = kiss_fftr_alloc(frameSize, 1, nullptr, nullptr);

  // the FFT buffer needs to cover up to N/2+1 float tuples (re,im)
  buffer = new kiss_fft_cpx[frameHalfSize + 1]();
}

FFT::~FFT()
{
  if (forward != nullptr)
  {
    free(forward);
    forward = nullptr;
  }

  if (backward != nullptr)
  {
    free(backward);
    backward = nullptr;
  }

  if (buffer != nullptr)
  {
    delete[] buffer;
    buffer = nullptr;
  }
}

inline void FFT::shift(float* frame) const
{
  const size_t offset = frameHalfSize;
  const size_t length = frameHalfSize * sizeof(float);

  // frame[0:N/2) => buffer
  memcpy(buffer, frame, length);

  // frame[N/2:N) => frame[0:N/2)
  memcpy(frame, frame + offset, length);

  // buffer => frame[N/2:N)
  memcpy(frame + offset, buffer, length);
}

inline void FFT::normalize(float* frame) const
{
  const float coeff = 1.0f / frameSize;

  for (size_t i = 0; i < frameSize; ++i)
  {
    frame[i] *= coeff;
  }
}


void FFT::fft(float* frame) const
{
  if (shiftFrame)
  {
    // circular shift by N/2
    shift(frame);
  }

  // fft(frame) => buffer
  kiss_fftr(forward, frame, buffer);

  // re(Nyquist) => im(DC)
  buffer[0].i = buffer[frameHalfSize].r;

  // buffer => frame
  memcpy(frame, buffer, frameSize * sizeof(float));
}

void FFT::ifft(float* frame) const
{
  // frame => buffer
  memcpy(buffer, frame, frameSize * sizeof(float));

  // im(DC) => re(Nyquist)
  buffer[frameHalfSize].r = buffer[0].i;
  buffer[0].i = buffer[frameHalfSize].i = 0;

  // ifft(buffer) => frame
  kiss_fftri(backward, buffer, frame);

  if (normalizeFrame)
  {
    // normalize transformed values by 1/N
    normalize(frame);
  }

  if (shiftFrame)
  {
    // circular shift by N/2
    shift(frame);
  }
}
