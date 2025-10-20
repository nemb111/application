#include "stdafx.h"

//#include "fresnelreflector.h"


//_____________________________________________________________________________
FresnelReflector::FresnelReflector(void)
{
}


//_____________________________________________________________________________
FresnelReflector::FresnelReflector(float etaIn, float etaOut)
  : _etaIn(etaIn), _etaOut(etaOut)
{
}


//_____________________________________________________________________________
FresnelReflector::~FresnelReflector(void)
{
}


//_____________________________________________________________________________
Spectrum
FresnelReflector::f(const ShadeRec& shadeRec, const Vector& wi,
                    const Vector& wo) const
{

  return BLACK;
}


//_____________________________________________________________________________
Spectrum
FresnelReflector::sampleF(const ShadeRec& shadeRec, Vector& wi,
                          const Vector& wo, float& pdf) const
{
  double ndotwo = shadeRec.normal * wo;
  wi = (-1. * wo) + 2.0 * shadeRec.normal * ndotwo;

  
  return Spectrum(WHITE) / static_cast<float>(fabs(shadeRec.normal * wi) );
}


//_____________________________________________________________________________
Spectrum
FresnelReflector::rho(const ShadeRec& shadeRec, const Vector& wo) const
{

  return BLACK;
}
  

//_____________________________________________________________________________
float 
FresnelReflector::fresnel(const ShadeRec& sr) const
{
  Normal normal = sr.normal;
  float cosThetaI = static_cast<float>(-1. * normal * sr.ray.d() );
  float eta;

  if (cosThetaI < 0.0) // ray hits inside surface
  {
    cosThetaI = static_cast<float>(-1. * cosThetaI);
    eta = _etaOut / _etaIn;
  }
  else
    eta = _etaIn / _etaOut;

  
  float tmp = 1.f - (1.f - cosThetaI * cosThetaI) / (eta * eta);

  if (tmp < 0.f)
    return 0.f;

  float cosThetaT = sqrt(tmp) ;
  float rParallel = (eta * cosThetaI - cosThetaT) / 
                    (eta * cosThetaI + cosThetaT);
  float rPerpendicular = (cosThetaI - eta * cosThetaT) /
                         (cosThetaI + eta * cosThetaT);
  float kr = 0.5f * (rParallel * rParallel + rPerpendicular * rPerpendicular);


  return kr;
}


//_____________________________________________________________________________
Spectrum
FresnelReflector::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                               Vector& wo, float& pdf) const
{
  double ndotwi = shadeRec.normal * wi;
  wo = (-1. * wi) + 2.0 * shadeRec.normal * ndotwi;
  pdf = 1.f;
  

  return Spectrum(WHITE) * fresnel(shadeRec);
}