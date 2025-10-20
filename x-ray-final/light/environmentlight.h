/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from ground up" / Seite 342 ff.
*/

#pragma once

#ifndef ENVIRONMENTLIGHT
#define ENVIRONMENTLIGHT


#include "light.h"

//Forward Deklarationen
class Emissive;
class HemisphereS;
class UnitSampler;

class EnvironmentLight : public Light
{
public:
  EnvironmentLight(/*const Emissive* emissiveMaterial,*/
                   float intensity, const Spectrum& color,
                   UnitSampler* uSampler,
                   bool castsShadow=true);
  virtual ~EnvironmentLight(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const;
  virtual Spectrum L(const ShadeRec& shadeRec);
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const;
  virtual Spectrum I() const {return _ce * _ke;}
  virtual const std::vector<const Point>& getSamplePoints(const ShadeRec& sr) const;

  virtual size_t numSamples() const;

protected:
  float _ke;
  Spectrum _ce;
  uint _numSamples;
  //const Emissive* _emissiveMaterial;
  HemisphereS* _hspheres;
};


#endif //ENVIRONMENTLIGHT