/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from ground up" / Seite 575 ff.
*/

#pragma once

#ifndef TRANSPARENT_H_
#define TRANSPARENT_H_

#include "phong.h"

//Forward Deklarationen
class PerfectSpecular;
class PerfectTransmitter;

class Transparent : public Phong
{
public:
  Transparent(Spectrum objC, Spectrum specC, double ambientRefl, double diffuseRefl,
              double specularRefl, double exp/*Phong*/, Spectrum perfSpecC,
              double perfSpecRefl/*PerfectSpecular*/, float transCoeff,
              float idxOfRefraction/*PerfectTransmitter*/);
  virtual ~Transparent(void);


  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:
  PerfectSpecular*    _pSpecularBRDF;
  PerfectTransmitter* _pTransmitterBTDF;

};

#endif//TRANSPARENT_H_