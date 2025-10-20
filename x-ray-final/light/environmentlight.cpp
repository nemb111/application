#include "stdafx.h"


//#include "emissive.h"
//#include "environmentlight.h"
//#include "hemispheres.h"
//#include "samplestorage.h"


//_____________________________________________________________________________
EnvironmentLight::EnvironmentLight(/*const Emissive* emissiveMaterial,*/
                                   float intensity, const Spectrum& color,
                                   UnitSampler* uSampler, 
                                   bool castsShadow/*=true*/)
 : Light(castsShadow), _ke(intensity), _ce(color),
   _numSamples(uSampler->getNumSamples() )
   /*_emissiveMaterial(emissiveMaterial)*/
{
  _hspheres = new HemisphereS(uSampler, 0.);

}


//_____________________________________________________________________________
EnvironmentLight::~EnvironmentLight(void)
{
  delete _hspheres;
  _hspheres = 0;
}


//_____________________________________________________________________________
Normal
EnvironmentLight::getDirection(const ShadeRec& shadeRec) const
{
  //Vorsicht quick and dirty Lösung: Der hier erhaltene Punkt (lightSamplePnt)
  //liegt noch auf einer Einheits-Hemisphäre (siehe hemispheres.h) und muss 
  //erst noch transformiert werden, damit man die tatsächliche Richtung bekommt

  Vector u, v(0.0034, 1, 0.0071);
  Vector w = shadeRec.normal;

  orthNormBasis(w, v, u);

  return Normal(v*shadeRec.lightSamplePnt.x + u*shadeRec.lightSamplePnt.y + 
                w*shadeRec.lightSamplePnt.z);
}


//_____________________________________________________________________________
Spectrum
EnvironmentLight::L(const ShadeRec& shadeRec)
{
  return _ke * _ce;
}


//_____________________________________________________________________________
bool
EnvironmentLight::inShadow(const Ray& ray, const ShadeRec& sr) const
{
  assert (sr.world.accelerator != NULL);

  if (_castsShadow == false)
    return false;

  double t;

  if (sr.world.accelerator->shadowHit(ray, t) )
    return true;
 
  return false;
}


//_____________________________________________________________________________
const std::vector<const Point>& 
EnvironmentLight::getSamplePoints(const ShadeRec& sr) const
{
  return *SStorage::getHSphSamples(_hspheres);
}


//_____________________________________________________________________________
size_t
EnvironmentLight::numSamples() const
{
  return _numSamples;
}