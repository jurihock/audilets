#include <Audilets/DSP/Algorithms/BernseeVocoder.h>

using namespace audilets::dsp::algorithms;

BernseeVocoder::BernseeVocoder(const size_t frameSampleRate, const size_t frameSize, const size_t frameHopRate) :
  frameSampleRate(frameSampleRate),
  frameSize(frameSize),
  frameHalfSize(frameSize / 2),
  frameHopRate(frameHopRate),
  stftFreqInc((float)frameSampleRate / (float)frameSize),
  stftFreqIncInv((float)frameSize / (float)frameSampleRate),
  stftPhaseInc((2.0f * math::pi) / (float)frameHopRate),
  stftPhaseIncInv((float)frameHopRate / (2.0f * math::pi))
{
  assert(frameSampleRate > 0);
  assert(frameSize > 0);
  assert(frameHopRate > 0);

  backedPhaseBuffer = new float[frameHalfSize]();
  accumPhaseBuffer = new float[frameHalfSize]();
}

BernseeVocoder::~BernseeVocoder()
{
  if (backedPhaseBuffer != nullptr)
  {
    delete[] backedPhaseBuffer;
    backedPhaseBuffer = nullptr;
  }

  if (accumPhaseBuffer != nullptr)
  {
    delete[] accumPhaseBuffer;
    accumPhaseBuffer = nullptr;
  }
}

void BernseeVocoder::analyze(std::complex<float>* fft)
{
  float magnitude,
        frequency,
        phase,
        delta,
        j;

  for (size_t i = 0; i < frameHalfSize; ++i)
  {
    magnitude = std::abs(fft[i]);
    phase = std::arg(fft[i]);

    delta = phase - backedPhaseBuffer[i];
    backedPhaseBuffer[i] = phase;

    j = math::wrap(delta - i * stftPhaseInc) * stftPhaseIncInv;

    frequency = (i + j) * stftFreqInc;

    fft[i].real(magnitude);
    fft[i].imag(frequency);
  }
}

void BernseeVocoder::synthesize(std::complex<float>* fft)
{
  float magnitude,
        frequency,
        phase,
        delta,
        j;

  for (size_t i = 0; i < frameHalfSize; ++i)
  {
    magnitude = fft[i].real();
    frequency = fft[i].imag();

    j = (frequency - i * stftFreqInc) * stftFreqIncInv;

    delta = (i + j) * stftPhaseInc;

    accumPhaseBuffer[i] += delta;
    phase = accumPhaseBuffer[i];

    fft[i] = std::polar(magnitude, phase);
  }
}
