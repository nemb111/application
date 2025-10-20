#include "stdafx.h"

//#include "perfecttransmitter.h"


//_____________________________________________________________________________
PerfectTransmitter::PerfectTransmitter(float transmissionCoeff, 
                                       float indexOfRefraction)
  : _kt(transmissionCoeff), _ior(indexOfRefraction)
{
}


//_____________________________________________________________________________
PerfectTransmitter::~PerfectTransmitter(void)
{
}


//_____________________________________________________________________________
Spectrum 
PerfectTransmitter::f(const ShadeRec& shadeRec, const Vector& wo,
                      const Vector& wt) const
{

  return BLACK;
}


//_____________________________________________________________________________
Spectrum 
PerfectTransmitter::sampleF(const ShadeRec& shadeRec, const Vector& wo,
                            Vector& wt) const
{
  Normal n = shadeRec.normal;
  float cosThetaI = static_cast<float>(n * wo);
  float eta = _ior;

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
  return (_kt / (eta * eta) * Spectrum(WHITE) / 
          fabs(static_cast<float>(shadeRec.normal * wt) ) );
}


//_____________________________________________________________________________
Spectrum
PerfectTransmitter::rho(const ShadeRec& shadeRec, const Vector& wo) const
{

  return BLACK;
}


//_____________________________________________________________________________
bool 
PerfectTransmitter::tir(const ShadeRec& shadeRec, const Vector& wo) const
{
  float cosThetaI = static_cast<float>(shadeRec.normal * wo);
  float eta = _ior;

  if (cosThetaI < 0.0f)
    eta = 1.0f / eta;

  return (1.0f - (1.0f - cosThetaI * cosThetaI) / (eta * eta) < 0.0f);
}


//_____________________________________________________________________________
Spectrum
PerfectTransmitter::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                 Vector& wt, float& pdf) const
{
  Normal n = shadeRec.normal;
  float cosThetaI = static_cast<float>(n * wi);
  float eta = _ior;

  if (cosThetaI < 0.0f)
  {
    cosThetaI = -cosThetaI;
    n = -1 * n;
    eta = 1.0f / eta;
  }

  float tmp = 1.0f - (1.0f - cosThetaI * cosThetaI) / (eta * eta);
  float cosTheta2 = sqrt(tmp);
  wt =  (-1 * wi) * (1.f / eta) - (cosTheta2 - cosThetaI / eta) * n;


  return (_kt * Spectrum(WHITE) );
}