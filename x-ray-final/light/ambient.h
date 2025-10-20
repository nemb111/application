/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 257ff.
*/


#pragma once

#ifndef AMBIENT_H_
#define AMBIENT_H_


#include "light.h"

class Ambient : public Light
{
public:
  Ambient(void);
  virtual ~Ambient(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const;
  virtual Spectrum L(const ShadeRec& shadeRec);
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const;
  virtual Spectrum I() const {return _spectrum * _ls;}

protected:
  float _ls;
  Spectrum _spectrum;
};

#endif//AMBIENT_H_