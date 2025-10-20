#include "stdafx.h"

//#include "boxfilter.h"


//_____________________________________________________________________________
BoxFilter::BoxFilter(uint xWidth, uint yWidth)
 : Filter(xWidth, yWidth), _invWeight(1.f / ((xWidth*2+1)*(yWidth*2+1)) )
{
  assert(_xWidth > 0 && _yWidth > 0);
}


//_____________________________________________________________________________
BoxFilter::~BoxFilter(void)
{
}


//_____________________________________________________________________________
Spectrum
BoxFilter::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
{
  Spectrum result(BLACK);

  for (int yw= (int)(-1*_yWidth); yw<=(int)_yWidth; yw++)
    for (int xw= (int)(-1*_xWidth); xw<=(int)_xWidth; xw++)
    {
      //if ((int)(yw+y) < 0 && (int)(xw+x) < 0)
      //  result += spec2D[calcNI(y, -yw)][calcNI(x, -xw)];
      //else if ((int)(yw+y) < 0 && (int)(xw+x) >= 0)
      //  result += spec2D[calcNI(y, -yw)][calcPI(x, xw, spec2D[0].size() )];
      //else if ((int)(yw+y) >= 0 && (int)(xw+x) < 0)
      //  result += spec2D[calcPI(y, yw, spec2D.size() )][calcNI(x, -xw)];
      //else
      //  result += spec2D[calcPI(y, yw, spec2D.size() )][calcPI(x, xw, spec2D[0].size() )];
      result += spec2D[calcI(y, yw, spec2D.size() )]
                      [calcI(x, xw, spec2D[y].size() )];
    }

  return _invWeight * result;
}