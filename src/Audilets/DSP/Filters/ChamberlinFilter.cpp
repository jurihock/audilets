#include <Audilets/DSP/Filters/ChamberlinFilter.h>

using namespace audilets::dsp::filters;

ChamberlinFilter::ChamberlinFilter(const size_t frameSampleRate, const float filterFrequency, const ChamberlinFilter::Type filterType) :
  frameSampleRate(frameSampleRate),
  filterFrequency(filterFrequency),
  filterType(filterType),
  f(2 * std::sin(math::pi * filterFrequency / frameSampleRate)),
  q(std::sqrt(2)),
  lp0(0),
  hp0(0),
  bp0(0)
{
}

ChamberlinFilter::~ChamberlinFilter()
{
}

float ChamberlinFilter::frequency() const
{
  return filterFrequency;
}

void ChamberlinFilter::frequency(const float value)
{
  filterFrequency = value;

  f = 2 * std::sin(math::pi * filterFrequency / frameSampleRate);
}

ChamberlinFilter::Type ChamberlinFilter::type() const
{
  return filterType;
}

void ChamberlinFilter::type(ChamberlinFilter::Type value)
{
  filterType = value;
}

float ChamberlinFilter::get(const ChamberlinFilter::Type value) const
{
  switch (value)
  {
    case ChamberlinFilter::Type::LP: return lp0;
    case ChamberlinFilter::Type::HP: return hp0;
    case ChamberlinFilter::Type::BP: return bp0;
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
  const auto hp1 = value + q * bp0 - lp0;
  const auto bp1 = f * hp1 + bp0;
  const auto lp1 = f * bp1 + lp0;

  hp0 = hp1;
  bp0 = bp1;
  lp0 = lp1;

  return get(filterType);
}
