/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 573 ff.
*/

#pragma once

#ifndef BTDF_H_
#define BTDF_H_

#include "ray.h"
#include "spectrum.h"

class BTDF
{
public:

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wo,
                     const Vector& wt) const = 0;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, const Vector& wo,
                           Vector& wt) const = 0;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const = 0;

  virtual bool     tir(const ShadeRec& shadeRec, const Vector& wo) const = 0;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wt, float& pdf) const
  {
    WOUT("Diese Methode muss von abgeleiteten Klassen implementiert werden.");
    return BLACK;
  }

};

#endif//BTDF_H_