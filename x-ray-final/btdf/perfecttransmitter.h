/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 573 ff.
*/

#pragma once

#ifndef PERFECTTRANSMITTER_H_
#define PERFECTTRANSMITTER_H_

#include "btdf.h"

class PerfectTransmitter : public BTDF
{
public:
  PerfectTransmitter(float transmissionCoeff, float indexOfRefraction);
  virtual ~PerfectTransmitter(void);

  virtual Spectrum f(const ShadeRec& shadeRec, const Vector& wo,
                     const Vector& wt) const;

  virtual Spectrum sampleF(const ShadeRec& shadeRec, const Vector& wo,
                           Vector& wt) const;

  virtual Spectrum rho(const ShadeRec& shadeRec, const Vector& wo) const;

  virtual bool     tir(const ShadeRec& shadeRec, const Vector& wo) const;

  virtual Spectrum photonSample(const ShadeRec& shadeRec, const Vector& wi,
                                Vector& wt, float& pdf) const;
 

protected:
  float _kt;
  float _ior;

};

#endif//PERFECTTRANSMITTER_H_