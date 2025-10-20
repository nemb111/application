/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 230ff.
*/


#pragma once

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_


#include "brdf.h"
#include "hemisphereS.h"
#include "unitsampler.h"


class Lambertian : public BRDF
{
public:
  Lambertian(void);
  Lambertian(const Spectrum& spectrum, double reflectance=1.0);
  Lambertian(const Spectrum& spectrum, UnitSampler* uSampler,
             double reflectance=1.0);
  virtual ~Lambertian(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wo, float& pdf) const;


  void setSpectrum(const Spectrum& spectrum);
  void setReflectance(double k);
  Spectrum getSpectrum(){return _cd;}
  double   getReflectance(){return _kd;}

protected:
  Spectrum _cd;
  UnitSampler* _unitSampler;
  HemisphereS _hemisphereS;
  double _kd;

};


#endif//LAMBERTIAN_H_