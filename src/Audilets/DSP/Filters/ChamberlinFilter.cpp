#include <Audilets/DSP/Filters/ChamberlinFilter.h>

using namespace audilets::dsp::filters;

ChamberlinFilter::ChamberlinFilter(const size_t frameSampleRate, const float filterFrequency, const ChamberlinFilter::Type filterType) :
  frameSampleRate(frameSampleRate),
  q(std::sqrt(2.0f)),
  value(0),
  lowpass(0),
  highpass(0),
  bandpass(0),
  bandstop(0)
{
  frequency(filterFrequency);
  type(filterType);
}

ChamberlinFilter::~ChamberlinFilter()
{
}

float ChamberlinFilter::frequency() const
{
  return filterFrequency;
}

void ChamberlinFilter::frequency(const float filterFrequency)
{
  this->filterFrequency = filterFrequency;

  f = 2 * std::sin(math::pi * filterFrequency / frameSampleRate);
}

ChamberlinFilter::Type ChamberlinFilter::type() const
{
  return filterType;
}

void ChamberlinFilter::type(ChamberlinFilter::Type filterType)
{
  this->filterType = filterType;
}

float ChamberlinFilter::get(const ChamberlinFilter::Type filterType) const
{
  switch (filterType)
  {
    case ChamberlinFilter::Type::Bypass:   return value;
    case ChamberlinFilter::Type::Lowpass:  return lowpass;
    case ChamberlinFilter::Type::Highpass: return highpass;
    case ChamberlinFilter::Type::Bandpass: return bandpass;
    case ChamberlinFilter::Type::Bandstop: return bandstop;
    default: throw std::runtime_error(
      "Unsupported Chamberlin filter type!");
  }
}

float ChamberlinFilter::get() const
{
  return get(filterType);
}

float ChamberlinFilter::set(float value)
{
  const auto highpass = value - q * this->bandpass - this->lowpass;
  const auto bandpass = f * highpass + this->bandpass;
  const auto lowpass = f * bandpass + this->lowpass;
  const auto bandstop = lowpass + highpass;

  this->value = value;
  this->lowpass = lowpass;
  this->highpass = highpass;
  this->bandpass = bandpass;
  this->bandstop = bandstop;

  return get(filterType);
}
