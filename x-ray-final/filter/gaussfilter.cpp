#include "stdafx.h"
#include "gaussfilter.h"


const double M_SQRT2 = std::sqrt(2.);

double erf(double x)
{
    // constants
  double a1 =  0.254829592;
  double a2 = -0.284496736;
  double a3 =  1.421413741;
  double a4 = -1.453152027;
  double a5 =  1.061405429;
  double p  =  0.3275911;

  // Save the sign of x
  int sign = 1;
  if (x < 0)
      sign = -1;
  x = fabs(x);

  // A&S formula 7.1.26
  double t = 1.0/(1.0 + p*x);
  double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

  return sign*y;
}


// Die Gaußsche Verteilungsfunktion
double normalCDF(double value, double sigma)
{
  return 0.5 * (1 + erf(value / (M_SQRT2 * sigma ) ) );
}


//_____________________________________________________________________________
GaussFilter::GaussFilter(float sigma)
  : Filter(static_cast<uint>(std::ceil(2 * sigma) ),
           static_cast<uint>(std::ceil(2 * sigma) ) )
{

  //Erzeugen des 1D Kernels
  _1DKernel.reserve(_xWidth * 2 + 1);


  double normalize = 0;
  for (int i=-static_cast<int>(_xWidth); i<=static_cast<int>(_xWidth); i++)
  {
    double integral = normalCDF(i - 0.5, sigma) - normalCDF(i + 0.5, sigma);
    normalize += integral;
    _1DKernel.push_back(integral );
  }


  for (size_t i=0; i<_1DKernel.size(); i++)
  {
    _1DKernel[i] /= normalize;
  }

}


//_____________________________________________________________________________
GaussFilter::~GaussFilter(void)
{
}


//_____________________________________________________________________________
void
GaussFilter::init(const Spectrum2D& spec2D)
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
        result += spec2D[y][calcI(x, i, xSize)] * 
                  static_cast<float>(_1DKernel[i+_xWidth]);
      }
      _hSpec2D[y][x] = result;
    }
  }
}


//_____________________________________________________________________________
Spectrum
GaussFilter::evaluate(const Spectrum2D& spec2D, uint x, uint y)const
{
  size_t ySize = spec2D.size();
  Spectrum result(BLACK);
  for (int i=-static_cast<int>(_yWidth); i<=static_cast<int>(_yWidth); i++)
  {
    result += _hSpec2D[calcI(y, i, ySize)][x] * 
              static_cast<float>(_1DKernel[i+_yWidth]);
  }
  

  return result;
}