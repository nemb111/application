/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 229ff.
*/


#pragma once

#ifndef SPECULAR_H_
#define SPECULAR_H_

#include "brdf.h"

class Specular : public BRDF
{
public:
  Specular(void);
  Specular(const Spectrum& spectrum, double reflectance=1.0, double exp=100);
  virtual ~Specular(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;


protected:
  double _ks;
  double _exp;
  Spectrum _cs;
  
};

#endif//SPECULAR_H_