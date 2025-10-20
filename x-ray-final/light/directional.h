/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 251ff.
*/


#pragma once

#ifndef DIRECTIONAL_H_
#define DIRECTIONAL_H_

#include "light.h"

class Directional : public Light
{
public:
  Directional(void);
  Directional(bool castsShadow);
  Directional(const Spectrum& color, const Vector& direction,
              float intensity=1., bool castsShadow=true);
  virtual ~Directional(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const;
  virtual Spectrum L(const ShadeRec& shadeRec);
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const;
  virtual Spectrum I() const {return _color * _intensity;}

protected:
  Spectrum _color;
  Normal _direction;
  float _intensity;


};

#endif//DIRECTIONAL_H_