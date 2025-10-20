/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 600 ff.
*/

#pragma once

#ifndef FRESNELTRANSMITTER_H
#define FRESNELTRANSMITTER_H

#include "btdf.h"


class FresnelTransmitter : public BTDF
{
public:
  //FresnelTransmitter(void);
  FresnelTransmitter(float etaIn, float etaOut);
  virtual ~FresnelTransmitter(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wo,
                     const Vector& wt) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, const Vector& wo,
                           Vector& wt) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo)
                       const;

  virtual bool tir(const ShadeRec& shadeRec, const Vector& wo) const;

  virtual float fresnel(const ShadeRec& sr) const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wt, float& pdf) const;


protected:
  float _etaIn, _etaOut;
};

#endif //FRESNELTRANSMITTER_H