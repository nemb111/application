#include "stdafx.h"
#include "boxfilter2.h"


//_____________________________________________________________________________
BoxFilter2::BoxFilter2(uint xWidth, uint yWidth)
  : Filter(xWidth, yWidth), _xInvWeight(1.f / (xWidth + xWidth + 1) ),
    _yInvWeight(1.f / (yWidth + yWidth + 1) )
{
}


//_____________________________________________________________________________
BoxFilter2::~BoxFilter2(void)
{
}


//_____________________________________________________________________________
void
BoxFilter2::init(const Spectrum2D& spec2D)
{
  size_t xSize = spec2D[0].size();
  size_t ySize = spec2D.size();
  _hSpec2D = Spectrum2D(ySize, std::vector<Spectrum>(xSize, 0) );


  for (size_t y=0; y<ySize; y++)
  {
    for (size_t x=0; x<xSize; x++)
    {
      Spectrum result(BLACK);
      for (int i=-static_cast<int>(_xWidth); i<=static_cast<int>(_xWidth); i++)
      {
        result += spec2D[y][calcI(x, i, xSize)];
      }
      _hSpec2D[y][x] = result * _xInvWeight;
    }
  }

}


//_____________________________________________________________________________
Spectrum
BoxFilter2::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
{
  size_t ySize = spec2D.size();
  Spectrum result(BLACK);
  for (int i=-static_cast<int>(_yWidth); i<=static_cast<int>(_yWidth); i++)
  {
    result += _hSpec2D[calcI(y, i, ySize)][x];
  }
  

  return result * _yInvWeight;
}