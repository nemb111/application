/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 500ff.
*/


#pragma once

#ifndef MIRROR_H_
#define MIRROR_H_

#include "perfectspecular.h"
#include "phong.h"

class Mirror : public Phong
{
public:
  Mirror(void);
  Mirror(Spectrum objC, Spectrum specC, Spectrum perfSpecC, double ambientRefl,
         double diffuseRefl, double specularRefl, double exp,
         double perfectSpecularRefl);
  ~Mirror(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;

  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  PerfectSpecular* _pSpecularBRDF;

};

#endif //MIRROR_H_