#pragma once

#ifndef SPECULARTEX_H_
#define SPECULARTEX_H_

#include "specular.h"


class SpecularTex : public Specular
{
public:
  SpecularTex(void);
  SpecularTex(const Spectrum& spectrum, double reflectance=1.0,
              double exp=100, const Texture* texture=NULL);
  virtual ~SpecularTex(void);

  
  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;


protected:
  const Texture* _cst;

};


#endif //SPECULARTEX_H_