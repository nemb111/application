/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 331 ff.
*/

#pragma once

#ifndef EMISSIVE_H_
#define EMISSIVE_H_

#include "hemispheres.h"
#include "material.h"


// Forward Deklarationen
class UnitSampler;



class Emissive : public Material
{
public:
  Emissive(void);
  Emissive(float intensityFront, const Spectrum& colorFront,
           float intensityBack, const Spectrum& colorBack,
           UnitSampler* unitSampler=nullptr);
  virtual ~Emissive(void);

  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum globalShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;

protected:

  void setGlobIlIntensity(float intensity) {_lsFrontGlobIl = intensity;}
  void setGlobIlColor(const Spectrum& spectrum) {_ceFrontGlobIl = spectrum;}

  float _lsFront;
  float _lsBack;
  float _lsFrontGlobIl;
  Spectrum _ceFront;
  Spectrum _ceBack;
  Spectrum _ceFrontGlobIl;

  UnitSampler* _unitSampler;
  HemisphereS _hemisphereS;



  friend class AreaLight;
};


#endif //EMISSIVE_H_