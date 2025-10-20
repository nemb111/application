#include "stdafx.h"
#include "boxfilter3.h"


//_____________________________________________________________________________
BoxFilter3::BoxFilter3(uint xWidth, uint yWidth)
  : Filter(xWidth, yWidth), _xInvWeight(1.f / (xWidth + xWidth + 1) ),
    _yInvWeight(1.f / (yWidth + yWidth + 1) )
{
}



//_____________________________________________________________________________
BoxFilter3::~BoxFilter3(void)
{
}


//_____________________________________________________________________________
void
BoxFilter3::init(const Spectrum2D& spec2D)
{
  size_t xSize = spec2D[0].size();
  size_t ySize = spec2D.size();
  Spectrum2D preSpec2D = Spectrum2D(ySize, std::vector<Spectrum>(xSize, 0) );
  _totSpec2D = Spectrum2D(ySize, std::vector<Spectrum>(xSize, 0) );


  // Die erste Spalte mit akkumulierten Werten ausfüllen
  for (size_t y=0; y<ySize; y++)
  {
    Spectrum result(BLACK);
    for (int xo=-static_cast<int>(_xWidth); xo<=static_cast<int>(_xWidth); xo++)
    {
      result += spec2D[y][calcI(0, xo, xSize)];
    }
    preSpec2D[y][0] = result * _xInvWeight;
  }


  // Jetzt alle Reihen ausfüllen
  for (size_t y=0; y<ySize; y++)
  {

    for (size_t x=1; x<xSize; x++)
    {
      size_t lmxMinus1 = calcI(x-1, -static_cast<int>(_xWidth), xSize);
      size_t rmx       = calcI(x, _xWidth, xSize);
      preSpec2D[y][x] = preSpec2D[y][x-1] - (spec2D[y][lmxMinus1] -
                         spec2D[y][rmx]) * _xInvWeight;
    }

  }


  // Nun die erste Reihe akkumulieren
  for (size_t x=0; x<xSize; x++)
  {
    Spectrum result(BLACK);
    for (int yo=-static_cast<int>(_yWidth); yo<=static_cast<int>(_yWidth); yo++)
    {
      result += preSpec2D[calcI(0, yo, ySize)][x];
    }
    _totSpec2D[0][x] = result * _yInvWeight;
  }


  // Nun alle Spalten ausfüllen
  for (size_t x=0; x<xSize; x++)
  {

    for (size_t y=1; y<ySize; y++)
    {
      size_t lmyMinus1 = calcI(y-1, -static_cast<int>(_yWidth), ySize);
      size_t rmy       = calcI(y, _yWidth, ySize);
      _totSpec2D[y][x] = _totSpec2D[y-1][x] - (preSpec2D[lmyMinus1][x] -
                         preSpec2D[rmy][x]) * _yInvWeight;
    }

  }

}


//_____________________________________________________________________________
Spectrum
BoxFilter3::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
{
  return _totSpec2D[y][x];

}