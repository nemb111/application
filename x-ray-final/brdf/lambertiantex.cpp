#include "stdafx.h"

//#include "lambertiantex.h"


//_____________________________________________________________________________
LambertianTex::LambertianTex(const Spectrum& spectrum, double reflectance,
                             const Texture* texture)
  : Lambertian(spectrum, reflectance), _cdt(texture)
{
}


//_____________________________________________________________________________
LambertianTex::~LambertianTex(void)
{
}


//_____________________________________________________________________________
Spectrum
LambertianTex::f(const ShadeRec& shadeRec, const Vector& wi, 
                 const Vector& wo) const
{
  
  // Verwende Alphablending um die Grundfarbe des Objekts mit der Textur zu
  // vermischen
  if (_cdt != NULL)
  {
    const RGBASpectrum rgba = _cdt->getSpectrum(shadeRec);
    float alpha = rgba.getAlpha();
    Spectrum cdt = rgba.getRGB();

    return ((alpha*cdt) + (1-alpha)*_cd) * static_cast<float>(_kd * INVPI);
  }
  else
    return _cd * static_cast<float>(_kd * INVPI);
}


//_____________________________________________________________________________
Spectrum
LambertianTex::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  
  // Verwende Alphablending um die Grundfarbe des Objekts mit der Textur zu
  // vermischen
  if (_cdt != NULL)
  {
    const RGBASpectrum rgba = _cdt->getSpectrum(shadeRec);
    float alpha = rgba.getAlpha();
    Spectrum cdt = rgba.getRGB();

    return static_cast<float>(_kd) * (alpha*cdt + (1-alpha)*_cd);
  }
  else
    return static_cast<float>(_kd) * _cd;
}


//_____________________________________________________________________________
Spectrum
LambertianTex::photonSample(const ShadeRec& shadeRec, const Vector& wi,
                            Vector& wo, float& pdf) const
{
  Normal w = shadeRec.normal;
  Vector u(-1.8473, 0.3183, 2.94729), v;
 
  orthNormBasis(w, u, v);

  const std::vector<const Point>* sp = 
    SStorage::getHSphSamples(&_hemisphereS, 1000);
  std::uniform_int_distribution<int> intDist(0, 
      static_cast<int>( sp->size() - 1) );
  int sampleIdx = MutexedRNG::number(intDist);


  wo = (*sp)[sampleIdx].x * u + (*sp)[sampleIdx].y * v + 
       (*sp)[sampleIdx].z * w;
  wo.normIP();

  pdf = static_cast<float>(shadeRec.normal * wo);


  if (_cdt != NULL)
  {
    const RGBASpectrum rgba = _cdt->getSpectrum(shadeRec);
    float alpha = rgba.getAlpha();
    Spectrum cdt = rgba.getRGB();

    return static_cast<float>(_kd) * (alpha*cdt + (1-alpha)*_cd);
  }
  else
    return _cd * static_cast<float>(_kd);
}