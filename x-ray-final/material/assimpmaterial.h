#pragma once

#ifndef ASSIMPMATERIAL_H_
#define ASSIMPMATERIAL_H_

#include "lambertiantex.h"
#include "material.h"
#include "speculartex.h"
#include "texture.h"

//Forward Deklarationen
class NormalMap;

class AssimpMaterial : public Material
{
public:
  AssimpMaterial(const Spectrum& ambientS, const Spectrum& diffuseS,
                 const Spectrum& specularS, double specExp, 
                 const Texture* ambientT=NULL,
                 const Texture* diffuseT=NULL, const Texture* specularT=NULL,
                 const NormalMap* normalMap=NULL);
  virtual ~AssimpMaterial(void);

  /*virtual Spectrum shade(ShadeRec& sr) const;*/
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum globalShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  LambertianTex* _ambientBRDF;
  LambertianTex* _diffuseBRDF;
  SpecularTex*   _specularBRDF;

  const NormalMap* _normalMap;
};


#endif //ASSIMPMATERIAL_H_