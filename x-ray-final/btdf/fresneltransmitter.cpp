#include "stdafx.h"

//#include "fresneltransmitter.h"


//_____________________________________________________________________________
//FresnelTransmitter::FresnelTransmitter(void)
//{
//}


//_____________________________________________________________________________
FresnelTransmitter::FresnelTransmitter(float etaIn, float etaOut)
  : _etaIn(etaIn), _etaOut(etaOut)
{
}


//_____________________________________________________________________________
FresnelTransmitter::~FresnelTransmitter(void)
{
}


//_____________________________________________________________________________
Spectrum 
FresnelTransmitter::f(const ShadeRec& shadeRec, const Vector& wo,
                      const Vector& wt) const
{

  return BLACK;
}


//_____________________________________________________________________________
Spectrum
FresnelTransmitter::sampleF(const ShadeRec& shadeRec, const Vector& wo,
                            Vector& wt) const
{
  Normal n = shadeRec.normal;
  float cosThetaI = static_cast<float>(n * wo);
  float eta = _etaIn / _etaOut;

  if (cosThetaI < 0.0f)
  {
    cosThetaI = -cosThetaI;
    n = -1 * n;
    eta = 1.0f / eta;
  }

  float tmp = 1.0f - (1.0f - cosThetaI * cosThetaI) / (eta * eta);
  float cosTheta2 = sqrt(tmp);
  wt =  (-1 * wo) * (1.f / eta) - (cosTheta2 - cosThetaI / eta) * n;


  // TODO: Gleichung umstellen um eine Division zu vermeiden
  return (1.f / (eta * eta) * Spectrum(WHITE) / 
          fabs(static_cast<float>(shadeRec.normal * wt) ) );
}


//_____________________________________________________________________________
Spectrum
FresnelTransmitter::rho(const ShadeRec& shadeRec, const Vector& wo) const
{

  return BLACK;
}


//_____________________________________________________________________________
bool
FresnelTransmitter::tir(const ShadeRec& shadeRec, const Vector& wo) const
{
  float cosThetaI = static_cast<float>(shadeRec.normal * wo);
  float eta = _etaIn / _etaOut;

  if (cosThetaI < 0.0f)
    eta = 1.0f / eta;

  return (1.0f - (1.0f - cosThetaI * cosThetaI) / (eta * eta) < 0.0f);
}


//_____________________________________________________________________________
float
FresnelTransmitter::fresnel(const ShadeRec& sr) const
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
  float kt = 1.f - (0.5f * (rParallel * rParallel + 
                            rPerpendicular * rPerpendicular) );


  return kt;
}


//_____________________________________________________________________________
Spectrum
FresnelTransmitter::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                 Vector& wt, float& pdf) const
{
  Normal n = shadeRec.normal;
  float cosThetaI = static_cast<float>(n * wi);
  float eta = _etaIn / _etaOut;

  if (cosThetaI < 0.0f)
  {
    cosThetaI = -cosThetaI;
    n = -1 * n;
    eta = 1.0f / eta;
  }

  float tmp = 1.0f - (1.0f - cosThetaI * cosThetaI) / (eta * eta);
  float cosTheta2 = sqrt(tmp);
  wt =  (-1 * wi) * (1.f / eta) - (cosTheta2 - cosThetaI / eta) * n;


  return (1.f / (eta * eta) * Spectrum(WHITE) * fresnel(shadeRec) );
}