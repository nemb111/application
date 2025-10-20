/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 257ff.
*/


#pragma once

#ifndef LIGHT_H_
#define LIGHT_H_

#include "linAlg.h"
#include "ray.h"
#include "spectrum.h"

//Forward Deklarationen
class Ray;
class ShadeRec;

class Light
{
public:
  Light(void);
  Light(bool castsShadow);
  virtual ~Light(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const = 0;
  virtual Spectrum L(const ShadeRec& shadeRec) = 0;
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const = 0;
  // Gibt Lichtintensität zurück. Physikalisch nicht korrekt aber liefert aber
  // immerhin ein Maß um mehrere Lichtquellen einer Art untereinander zu
  // vergleichen.
  virtual Spectrum I() const = 0;
  //virtual Point getSamplePoint(const ShadeRec& sr) {return Point(0,0,0);}
  virtual const std::vector<const Point>& getSamplePoints(const ShadeRec& sr) const;
  virtual float G(const ShadeRec& sr) const {return 1.f;}
  virtual float pdf(const ShadeRec& sr) const {return 1.f;}
 
  virtual bool castsShadow() const;
  // TODO: Diese Methoden sind eher schlecht, da Punktlichter über keine
  // Sampler verfügen.
  virtual size_t numSamples() const {return 1;}
  //virtual void resetSampler() {}

protected:
  bool _castsShadow;


private:
  std::vector<const Point> _samplePoint;

};

#endif//LIGHT_H_