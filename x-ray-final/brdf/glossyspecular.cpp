#include "stdafx.h"

//#include "glossyspecular.h"


//_____________________________________________________________________________
GlossySpecular::GlossySpecular(void)
  : _exp(1.), _kg(1.), _cg(WHITE)
{
}


//_____________________________________________________________________________
GlossySpecular::GlossySpecular(const Spectrum& spectrum,
                               double reflectance/*=1.*/, double exp/*=1.*/)
  : _exp(exp), _kg(reflectance), _cg(spectrum)
{
}


//_____________________________________________________________________________
GlossySpecular::~GlossySpecular(void)
{
}


//_____________________________________________________________________________
Spectrum
GlossySpecular::f(const ShadeRec& shadeRec, const Vector& wi,
                  const Vector& wo) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
GlossySpecular::sampleF(const ShadeRec& shadeRec, Vector& wi,
                        const Vector& wo, float& pdf) const
{
  double ndotw = wo * shadeRec.normal;
  Vector r = (-1 * wo) + 2.0 * shadeRec.normal * ndotw;
  double phongLobe = std::pow(r * wi, static_cast<FLOATING>(_exp) );
  pdf = static_cast<float>(phongLobe * (shadeRec.normal * wi) );


  return _cg * static_cast<float>(_kg * phongLobe);
}


//_____________________________________________________________________________
Spectrum
GlossySpecular::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
GlossySpecular::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                             Vector& wo, float& pdf) const
{
  Vector u(-1.8473, 0.3183, 2.94729), v;
  double ndotwi = wi * shadeRec.normal;
  // w = Richtung der perfekten Reflektion
  Vector w = -1. * wi + 2.0 * shadeRec.normal * ndotwi;
  orthNormBasis(w, u, v);

  const std::vector<const Point>* samplePoints =
    SStorage::getHSphSamples(_hemisphereS);

  std::uniform_int_distribution<size_t> uniSPointDist(0, samplePoints->size() - 1 );
  size_t rndPointIdx = MutexedRNG::number(uniSPointDist);
  const Point& samplePoint = (*samplePoints)[rndPointIdx];


  wo =  u*samplePoint.x + v*samplePoint.y + w*samplePoint.z;


  // Falls der reflektierte Strahl in die Geometrie deutet spiegle ihn entlang
  // der perfekten Reflexion
  if (shadeRec.normal * wo < 0.0)
    wo = u*-samplePoint.x + v*-samplePoint.y + w*samplePoint.z;


  double phongLobe = std::pow(w * wo, static_cast<FLOATING>(_exp) );
  pdf = static_cast<float>(phongLobe * (shadeRec.normal * wi) );


  return _cg * static_cast<float>(_kg);
}


//_____________________________________________________________________________
void
GlossySpecular::setHemisphereS(const HemisphereS* hemisphereS)
{
  _hemisphereS = hemisphereS;
}