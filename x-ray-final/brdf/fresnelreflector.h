/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 600 ff.
*/


#pragma once

#ifndef FRESNELREFLECTOR_H_
#define FRESNELREFLECTOR_H_

#include "brdf.h"
class FresnelReflector : public BRDF
{
public:
  FresnelReflector(void);
  FresnelReflector(float etaIn, float etaOut);
  virtual ~FresnelReflector(void);


  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;

  virtual float fresnel(const ShadeRec& sr) const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wo, float& pdf) const;
  

protected:
  float _etaIn, _etaOut;
};


#endif //FRESNELREFLECTOR_H_