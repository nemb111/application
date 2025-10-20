/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 535ff.
*/


#pragma once

#ifndef REALISTICMIRROR_H_
#define REALISTICMIRROR_H_

#include "hemispheres.h"
#include "glossyspecular.h"
#include "material.h"
#include "phong.h"

class RealisticMirror : public Phong
{
public:
  RealisticMirror(void);
  RealisticMirror(Spectrum objC, Spectrum specC, Spectrum glossySpecC,
                    double ambientRefl, double diffuseRefl,
                    double specularRefl, double exp, 
                    double glossySpecularRefl, UnitSampler* unitSampler);
  virtual ~RealisticMirror(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;

  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  bool _createdSampler;
  double _exp;
  GlossySpecular* _gSpecularBRDF;
  UnitSampler* _unitSampler;
  HemisphereS* _hsphere;
};

#endif //REALISTICMIRROR_H_