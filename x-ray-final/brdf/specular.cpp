#include "stdafx.h"

//#include "specular.h"


//_____________________________________________________________________________
Specular::Specular(void)
  : _ks(1.), _exp(100.), _cs(WHITE)
{
}


//_____________________________________________________________________________
Specular::Specular(const Spectrum& spectrum, double reflectance/*=1.0*/, 
                   double exp/*=100*/)
  : _ks(reflectance), _exp(exp), _cs(spectrum)
{
}


//_____________________________________________________________________________
Specular::~Specular(void)
{
}


//_____________________________________________________________________________
Spectrum 
Specular::f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const
{
  assert(gutils::equals(wi.length2(), 1.) );
  assert(gutils::equals(shadeRec.normal.length2(), 1.) );


  double ndotwi = shadeRec.normal * wi;
  Vector r(-1 * wi + 2.0 * shadeRec.normal * ndotwi);
  double rcos = r * wo;
  //assert(rcos >= 0 && rcos <= 1);

  if (rcos > 0.0)
    return _cs * static_cast<float>(_ks * pow(rcos, _exp) );

  return BLACK;
}


//_____________________________________________________________________________
Spectrum 
Specular::sampleF(const ShadeRec& shadeRec, Vector& wi, const Vector& wo,
                  float& pdf) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
Specular::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  return BLACK;
}


//_____________________________________________________________________________



//_____________________________________________________________________________



//_____________________________________________________________________________