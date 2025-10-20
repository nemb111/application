#include "stdafx.h"

//#include <random>
//
//#include "lambertian.h"
//#include "mutexedrng.h"
//#include "options.h"
//#include "parser.h"
//#include "samplestorage.h"
//#include "stratifieds.h"

//_____________________________________________________________________________
Lambertian::Lambertian(void)
  : _cd(WHITE), _kd(1.)
{
  _unitSampler = new StratifiedS(100);
  Parser::GB.mrkDel<UnitSampler*>(_unitSampler);
  _hemisphereS = HemisphereS(_unitSampler, 1.0) ;
}


//_____________________________________________________________________________
Lambertian::Lambertian(const Spectrum& spectrum, double reflectance/*=1.0*/)
  : _cd(spectrum), _kd(reflectance)
{
   _unitSampler = new StratifiedS(100);
  Parser::GB.mrkDel<UnitSampler*>(_unitSampler);
  _hemisphereS = HemisphereS(_unitSampler, 1.0) ;
}


//_____________________________________________________________________________
Lambertian::Lambertian(const Spectrum& spectrum, UnitSampler* uSampler,
                       double reflectance/*=1.0*/)
  : _cd(spectrum), _unitSampler(uSampler), _kd(reflectance)
{
  _hemisphereS = HemisphereS(_unitSampler, 1.0);
}


//_____________________________________________________________________________
Lambertian::~Lambertian(void)
{
}


//_____________________________________________________________________________
Spectrum
Lambertian::f(const ShadeRec& shadeRec, const Vector& wi,
              const Vector& wo) const
{
  return _cd * static_cast<float>(_kd * INVPI);
}


//_____________________________________________________________________________
Spectrum
Lambertian::sampleF(const ShadeRec& shadeRec, Vector& wi,
                    const Vector& wo, float& pdf) const
{
  Normal w = shadeRec.normal;
  Vector u(-1.8473, 0.3183, 2.94729), v;
 
  orthNormBasis(w, u, v);

  const std::vector<const Point>* sp = 
    SStorage::getHSphSamples(&_hemisphereS, 1000);
  std::uniform_int_distribution<int> intDist(0, 
      static_cast<int>( sp->size() - 1) );
  int sampleIdx = MutexedRNG::number(intDist);


  wi = (*sp)[sampleIdx].x * u + (*sp)[sampleIdx].y * v + 
       (*sp)[sampleIdx].z * w;
  wi.normIP();

  pdf = static_cast<float>(shadeRec.normal * wi);


  return _cd * static_cast<float>(_kd * INVPI);
}


//_____________________________________________________________________________
Spectrum
Lambertian::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  return static_cast<float>(_kd) * _cd;
}


//_____________________________________________________________________________
Spectrum 
Lambertian::photonSample(const ShadeRec& shadeRec, const Vector& wi,
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


  return _cd * static_cast<float>(_kd);
}


//_____________________________________________________________________________
void
Lambertian::setSpectrum(const Spectrum& spectrum)
{
  _cd = spectrum;
}


//_____________________________________________________________________________
void
Lambertian::setReflectance(double k)
{
  _kd = k;
}