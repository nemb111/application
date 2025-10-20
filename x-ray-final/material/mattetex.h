#pragma once

#ifndef MATTETEX_H_
#define MATTETEX_H_


#include "lambertiantex.h"
#include "material.h"
#include "texture.h"


class MatteTex : public Material
{
public:
  MatteTex(Spectrum spectrum, double ambientRefl, double diffuseRefl,
           const Texture* texture);
  MatteTex(const Spectrum& ambientS, const Texture* ambientT,
           const Spectrum& diffuseS, const Texture* diffuseT);
  virtual ~MatteTex(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;

protected:
  LambertianTex* _ambientBRDF;
  LambertianTex* _diffuseBRDF;
};

#endif //MATTETEX_H_