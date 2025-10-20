#pragma once

#ifndef LAMBERTIANTEX_H_
#define LAMBERTIANTEX_H_


#include "lambertian.h"
#include "texture.h"


class LambertianTex : public Lambertian
{
public:
  LambertianTex(const Spectrum& spectrum, double reflectance,
                const Texture* texture);
  virtual ~LambertianTex(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wo, float& pdf) const;

protected:
  const Texture* _cdt;
};

#endif //LAMBERTIANTEX_H_