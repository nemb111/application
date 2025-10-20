#pragma once

#ifndef DEBUGMATERIAL_H_
#define DEBUGMATERIAL_H_

#include "material.h"

//Forward Deklaration
class Material;

class DebugMaterial : public Material
{
public:
  DebugMaterial(void);
  DebugMaterial(const Spectrum& color);
  virtual ~DebugMaterial(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;

protected:
  Spectrum _color;
};

#endif//DEBUGMATERIAL_H_