#include "stdafx.h"

//#include "nofilter.h"


//_____________________________________________________________________________
NoFilter::NoFilter(void)
  : Filter(1,1)
{
}


//_____________________________________________________________________________
NoFilter::~NoFilter(void)
{
}


//_____________________________________________________________________________
Spectrum
NoFilter::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
{
  return spec2D[y][x];
}