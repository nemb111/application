/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 270 ff.
*/


#pragma once

#ifndef MATTE_H_
#define MATTE_H_

#include "lambertian.h"
#include "material.h"


class Matte : public Material
{
public:
  Matte(void);
  Matte(Spectrum spectrum, double ambientRefl, double diffuseRefl);
  //Spezieller Konstruktor um absolute Farbwerte zu setzen. Der reflectance
  //wird auf 1. gesetzt
  Matte(const Spectrum& ambientS, const Spectrum& diffuseS);
  virtual ~Matte(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum globalShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  Lambertian* _ambientBRDF;
  Lambertian* _diffuseBRDF;
};

#endif//MATTE_H_