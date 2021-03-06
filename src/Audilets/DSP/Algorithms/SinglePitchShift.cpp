#include <Audilets/DSP/Algorithms/SinglePitchShift.h>

using namespace audilets::dsp::algorithms;

SinglePitchShift::SinglePitchShift(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate) :
  STFT(frameSampleRate, frameSize, frameHopRate),
  vocoder(frameSampleRate, frameSize, frameHopRate),
  pitchShiftingFactor(1)
{
  frameBuffer = new std::complex<float>[frameHalfSize]();
}

SinglePitchShift::~SinglePitchShift()
{
  if (frameBuffer != nullptr)
  {
    delete[] frameBuffer;
    frameBuffer = nullptr;
  }
}

float SinglePitchShift::getPitchShiftingFactor() const
{
  return pitchShiftingFactor;
}

void SinglePitchShift::setPitchShiftingFactor(const float value)
{
  assert(0.5 <= value && value <= 2);
  pitchShiftingFactor = value;
}

void SinglePitchShift::processInTimeDomain(float* frame)
{
}

void SinglePitchShift::processInFrequencyDomain(std::complex<float>* frame)
{
  vocoder.analyze(frame);
  process(frame);
  vocoder.synthesize(frame);
}

void SinglePitchShift::process(std::complex<float>* frame)
{
  float magnitude,
        frequency;

  memset(frameBuffer, 0, frameSize * sizeof(float));

  for (size_t i = 1; i < frameHalfSize; ++i)
  {
    const size_t j = i * pitchShiftingFactor;

    if (j <= 0 || frameHalfSize <= j) continue;

    magnitude = std::real(frame[i]);
    frequency = std::imag(frame[i]);

    magnitude += std::real(frameBuffer[j]);
    frequency *= pitchShiftingFactor;

    frameBuffer[j].real(magnitude);
    frameBuffer[j].imag(frequency);
  }

  memcpy(frame, frameBuffer, frameSize * sizeof(float));
}
