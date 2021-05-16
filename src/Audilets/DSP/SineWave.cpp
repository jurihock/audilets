#include "SineWave.h"

using namespace audilets::dsp;

SineWave::SineWave(const size_t sampleRate, const size_t waveFrequency)
{
  setSampleRate(sampleRate);
  setWaveFrequency(waveFrequency);
}

SineWave::~SineWave()
{
}

size_t SineWave::getSampleRate() const
{
  return sampleRate;
}

void SineWave::setSampleRate(const size_t sampleRate)
{
  assert(sampleRate > 0);

  this->sampleRate = sampleRate;

  phaseDeviation = 1.0f * waveFrequency / sampleRate;
  phaseDivisor = -phaseDeviation;
}

size_t SineWave::getWaveFrequency() const
{
  return waveFrequency;
}

void SineWave::setWaveFrequency(const size_t waveFrequency)
{
  assert(waveFrequency > 0);

  this->waveFrequency = waveFrequency;

  phaseDeviation = 1.0f * waveFrequency / sampleRate;
  phaseDivisor = -phaseDeviation;
}

float SineWave::getNextPhase()
{
  phaseDivisor += phaseDeviation;

  while(phaseDivisor >= 1)
    phaseDivisor -= 1;

  return 2.0f * math::pi * phaseDivisor;
}

float SineWave::getNextValue()
{
  return std::sin(getNextPhase());
}
