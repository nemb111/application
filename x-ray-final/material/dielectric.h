/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 600 ff.
*/

#pragma once

#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "phong.h"


// Forward Deklarationen
class FresnelReflector;
class FresnelTransmitter;

class Dielectric : public Phong
{
public:
  //Dielectric(void);
  Dielectric(const Spectrum& objC, const Spectrum& specC, double ambientRefl,
             double diffuseRefl, double specularRefl, double exp,
             float etaOuterior, float etaInterior,
             const Spectrum& inColorFilter, const Spectrum& outColorFilter,
             float dOutCoeff=1.f, float dInCoeff=1.f);
  virtual ~Dielectric(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  float _dOutCoeff;
  float _dInCoeff;

  Spectrum _cfOut;  // exterior filter color
  Spectrum _cfIn;   // interior filter color
 

  const FresnelReflector*   _fresnelRefBRDF;
  const FresnelTransmitter* _fresnelTransBTDF;

};

#endif //DIELECTRIC_H_