/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 229ff.
*/


#pragma once

#ifndef BRDF_H_
#define BRDF_H_

#include "ray.h"
#include "spectrum.h"

class BRDF
{
public:

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const = 0;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const = 0;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const = 0;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wo, float& pdf) const
  {
    WOUT("Diese Methode muss von abgeleiteten Klassen implementiert werden.");
    return BLACK;
  }


protected:


};



#endif//BRDF_H_