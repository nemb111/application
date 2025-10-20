/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 258ff.
*/


#pragma once

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "debugmaterial.h"
#include "debugsphere.h"
#include "instance.h"
#include "light.h"

class PointLight : public Light
{
public:
  PointLight(void);
  PointLight(bool castsShadow);
  PointLight(const Point& position, const Spectrum& color=WHITE,
             float intensity=1., bool castsShadow=true);
  virtual ~PointLight(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const;
  virtual Spectrum L(const ShadeRec& shadeRec);
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const;
  virtual Spectrum I() const {return _color * _intensity;}
  virtual void showSphere(World& world);
  virtual void showSphere(World& world, DebugSphere* dsphere,
                          const Spectrum& color);

protected:
  Point _position;
  Spectrum _color;
  float _intensity;
  DebugSphere* _dsphere;
  DebugMaterial* _dmaterial;
  Instance* _instance;
};

#endif//POINTLIGHT_H_