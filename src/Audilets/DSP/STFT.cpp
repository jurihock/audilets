#include "STFT.h"

using namespace Audilets::DSP;

STFT::STFT(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate) :
  frameSampleRate(frameSampleRate),
  frameSize(frameSize),
  frameHalfSize(frameSize / 2),
  frameHopRate(frameHopRate),
  frameHopSize(frameSize / frameHopRate)
{
  assert(frameSampleRate > 0);
  assert(frameSize > 0);
  assert(frameHopRate > 0);

  analysisBuffer = new short[frameSize * 2]();
  synthesisBuffer = new short[frameSize * 2]();

  frameBuffer = new float[frameSize]();
  frameWindow = new float[frameSize]();

  Window(frameSize, frameHopRate, /*periodic*/true, /*weighted*/true).hann(frameWindow);

  for (size_t i = frameHopRate; i > 0; --i)
  {
    const size_t hop = frameSize - frameHopSize * (i - 1);

    frameHops.push_back(hop);
  }

  fft = new FFT(frameSize, /*shift*/true, /*normalize*/true);
}

STFT::~STFT()
{
  if (analysisBuffer != nullptr)
  {
    delete[] analysisBuffer;
    analysisBuffer = nullptr;
  }

  if (synthesisBuffer != nullptr)
  {
    delete[] synthesisBuffer;
    synthesisBuffer = nullptr;
  }

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
}

void STFT::stft(const short* frame, short* reframe)
{
  // shift the analysis buffer by frame size
  memcpy(analysisBuffer, analysisBuffer + frameSize, frameSize * sizeof(short));

  // copy the input frame into the analysis buffer
  memcpy(analysisBuffer + frameSize, frame, frameSize * sizeof(short));

  // analyze and synthesize the buffered data hop by hop
  for (const size_t frameHop : frameHops)
  {
    // extract the current subframe for analysis
    Convert::fromShortToFloat(analysisBuffer + frameHop, frameBuffer, frameWindow, frameSize);

    // process the extracted subframe in time domain
    processInTimeDomain(frameBuffer);

    // process the extracted subframe in frequency domain
    fft->fft(frameBuffer);
    processInFrequencyDomain(reinterpret_cast<std::complex<float>*>(frameBuffer));
    fft->ifft(frameBuffer);

    // synthesize the processed subframe
    Convert::fromFloatToShortPlus(frameBuffer, synthesisBuffer + frameHop, frameWindow, frameSize);
  }

  // copy the finished frame into the output frame
  memcpy(reframe, synthesisBuffer, frameSize * sizeof(short));

  // shift the synthesis buffer by frame size
  memcpy(synthesisBuffer, synthesisBuffer + frameSize, frameSize * sizeof(short));

  // initialize the next synthesis frame
  memset(synthesisBuffer + frameSize, 0, frameSize * sizeof(short));
}
