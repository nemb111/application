#include "stdafx.h"

//#include "mitchellfilter.h"


//_____________________________________________________________________________
MitchellFilter::MitchellFilter(uint xWidth, uint yWidth, float B/*=0.33333*/,
                               float C/*=0.33333*/)
 : Filter(xWidth, yWidth), _B(B), _C(C), _invXWidth(1.f/(xWidth*.5f) ),
   _invYWidth(1.f/(yWidth*.5f) )
{
  assert(0 <= _B && 0 <= _C);
  assert(_B <= 1 && _C <= 1);
}


//_____________________________________________________________________________
MitchellFilter::~MitchellFilter(void)
{
}


//_____________________________________________________________________________
Spectrum
MitchellFilter::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
{
  Spectrum result(BLACK);
  float weight = 0;
  float sum = 0;
  int xWidth = _xWidth, yWidth = _yWidth;
  size_t xIndex, yIndex;

  for (int yw= -yWidth; yw<=yWidth; yw++)
    for (int xw= -xWidth; xw<=xWidth; xw++)
    {
      xIndex = calcI(x, xw, spec2D[0].size() );
      yIndex = calcI(y, yw, spec2D.size() );
      weight = Mitchell1D(xw * _invXWidth) * Mitchell1D(yw * _invYWidth);
      result += weight * spec2D[yIndex][xIndex];
      sum += weight;
    }

  for (int i=0; i<result.numSamples; i++)
    if (result[i] < 0.f)
      result[i] = 0.f;

  return result / sum;
}