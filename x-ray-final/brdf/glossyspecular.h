/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 229ff.
*/


#pragma once

#ifndef GLOSSYSPECULAR_H_
#define GLOSSYSPECULAR_H_

#include "brdf.h"
#include "hemispheres.h"


class GlossySpecular : public BRDF
{
public:
  GlossySpecular(void);
  GlossySpecular(const Spectrum& spectrum, double refelectance=1.0,
                 double exp=1.0);
  virtual ~GlossySpecular(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wi,
                     const Vector& wo) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, Vector& wi,
                           const Vector& wo, float& pdf) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wo, float& pdf) const;

protected:
  double _exp;
  double _kg;
  Spectrum _cg;
  const HemisphereS* _hemisphereS;

  // Wird nur von RealisticMirror benötigt, damit die BRDF die Ausfallrichtung
  // des Photons bestimmen kann.
  void setHemisphereS(const HemisphereS* hemisphereS);



  friend class RealisticMirror;

};

#endif //GLOSSYSPECULAR_H_