#include "stdafx.h"

#include <boost/math/special_functions/round.hpp>

#include "boxedgaussfilter.h"


//function boxesForGauss(sigma, n)  // standard deviation, number of boxes
//{
//    var wIdeal = Math.sqrt((12*sigma*sigma/n)+1);  // Ideal averaging filter width 
//    var wl = Math.floor(wIdeal);  if(wl%2==0) wl--;
//    var wu = wl+2;
//				
//    var mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n)/(-4*wl - 4);
//    var m = Math.round(mIdeal);
//    // var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );
//				
//    var sizes = [];  for(var i=0; i<n; i++) sizes.push(i<m?wl:wu);
//    return sizes;
//}


void boxesForGauss(float sigma, int iteration, vector<uint>& widths)
{
  double wIdeal = std::sqrt((12.*sigma*sigma/iteration)+1); // Ideal averaging filter width 
  int wl = static_cast<int>(std::floor(wIdeal) );
  if (wl % 2 == 0) wl--;
  int wu = wl+2;

  double mIdeal = (12.*sigma*sigma - iteration*wl*wl - 4.*iteration*wl -
                   3.*iteration) / (-4.*wl - 4.);
  double m = boost::math::round(mIdeal);

  widths.clear();
  for (int i=0; i<iteration; i++)
  {
    widths.push_back(i<m?wl:wu);
  }

}


//_____________________________________________________________________________
BoxedGaussFilter::BoxedGaussFilter(float sigma, uint iteration/*=6*/)
  : BoxFilter3(1, 1), _sigma(sigma), _iteration(iteration)
{

}


//_____________________________________________________________________________
BoxedGaussFilter::~BoxedGaussFilter(void)
{
}


//_____________________________________________________________________________
void
BoxedGaussFilter::init(const Spectrum2D& spec2D)
{
  vector<uint> widths;
  boxesForGauss(_sigma, _iteration, widths);

  _xWidth = _yWidth = (widths[0] - 1) / 2;
  _xInvWeight = 1.f / (2*_xWidth + 1);
  _yInvWeight = 1.f / (2*_yWidth + 1);
  BoxFilter3::init(spec2D);

  for (uint i=1; i<_iteration; i++)
  {
    _xWidth = _yWidth = (widths[i] - 1) / 2;
    _xInvWeight = 1.f / (2*_xWidth + 1);
    _yInvWeight = 1.f / (2*_yWidth + 1);
    Spectrum2D tmp = std::move(_totSpec2D);
    BoxFilter3::init(tmp);
  }

}


//_____________________________________________________________________________
//Spectrum 
//BoxedGaussFilter::evaluate(const Spectrum2D& spec2D, uint x, uint y) const
//{
//
//  return BLACK;
//}
