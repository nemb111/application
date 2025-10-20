/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 284ff.
*/


#pragma once

#ifndef PHONG_H_
#define PHONG_H_

#include "lambertian.h"
#include "material.h"
#include "specular.h"



class Phong : public Material
{
public:
  Phong(void);
  Phong(Spectrum objC, Spectrum specC, double ambientRefl, double diffuseRefl,
        double specularRefl, double exp);
  virtual ~Phong(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  Lambertian* _ambientBRDF;
  Lambertian* _diffuseBRDF;
  Specular*   _specularBRDF;

};

#endif//PHONG_H_