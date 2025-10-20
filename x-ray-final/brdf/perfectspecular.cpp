#include "stdafx.h"

//#include "perfectspecular.h"


//_____________________________________________________________________________
PerfectSpecular::PerfectSpecular(void)
  : _kp(1.), _cp(WHITE)
{
}


//_____________________________________________________________________________
PerfectSpecular::PerfectSpecular(const Spectrum& spectrum, double reflectance/*=1.0*/)
  : _kp(reflectance), _cp(spectrum)
{
}


//_____________________________________________________________________________
PerfectSpecular::~PerfectSpecular(void)
{
}


//_____________________________________________________________________________
Spectrum 
PerfectSpecular::f(const ShadeRec& shadeRec, const Vector& wi,
                   const Vector& wo) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum 
PerfectSpecular::sampleF(const ShadeRec& shadeRec, Vector& wi, 
                         const Vector& wo, float& pdf) const
{
  double ndotwo = shadeRec.normal * wo;
  wi = (-1. * wo) + 2.0 * shadeRec.normal * ndotwo;
  pdf = 1;
  
  return (_cp * static_cast<float>(_kp / (fabs(shadeRec.normal * wi) ) ) );
}


//_____________________________________________________________________________
Spectrum
PerfectSpecular::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
PerfectSpecular::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                              Vector& wo, float& pdf) const
{
  double ndotwi = shadeRec.normal * wi;
  wo = (-1. * wi) + 2.0 * shadeRec.normal * ndotwi;
  pdf = 1;
  

  return (_cp * static_cast<float>(_kp) );
}