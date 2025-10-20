#include "stdafx.h"

//#include "arealight.h"
//#include "samplestorage.h"
//#include "photonrenderer.h"

//_____________________________________________________________________________
//AreaLight::AreaLight(void)
//{
//}


//_____________________________________________________________________________
AreaLight::AreaLight(float intensity,
                     const Spectrum& color,
                     LightShape* lightShape, 
                     const Emissive* emissiveMaterial, UnitSampler* uSampler,
                     World& world, bool showLightShape/*=true*/, 
                     bool castsShadow/*=true*/)
  : Light(castsShadow), _ka(intensity), _pka(0.f),
    _ca(color), _lightShape(lightShape),
    _emissiveMaterial(emissiveMaterial)
{
  assert(_lightShape);
  assert(_emissiveMaterial);
  assert(uSampler);

  // emissiveMaterial muss mit angepassten Farb- und Intensitätswerten 
  // ausgestattet werden, da bei globaler Beleuchtung nicht mehr die Fläche-
  // sondern Hemisphärform der LTE verwendet wird
  const_cast<Emissive*>(_emissiveMaterial)->setGlobIlIntensity
    (_ka / _lightShape->getArea() );
  const_cast<Emissive*>(_emissiveMaterial)->setGlobIlColor(_ca);

  _lightShape->setUnitSampler(uSampler);
  _lightShape->setMaterial(_emissiveMaterial);
  _lightShape->setThrowsShadow(false);
  if (showLightShape == true)
    world.addShape(_lightShape);

  /*_lightShape->resetSampler();
  for (size_t i=0; i<_lightShape->numSamples(); i++)
    _samplePoints.push_back(_lightShape->getSample() );*/

}


//_____________________________________________________________________________
AreaLight::AreaLight(float intensity, float photonIntensity,
                     const Spectrum& color,
                     LightShape* lightShape, 
                     const Emissive* emissiveMaterial, UnitSampler* uSampler,
                     World& world, bool showLightShape/*=true*/, 
                     bool castsShadow/*=true*/)
  : Light(castsShadow), _ka(intensity), _pka(photonIntensity),
    _ca(color), _lightShape(lightShape),
    _emissiveMaterial(emissiveMaterial)
{
  assert(_lightShape);
  assert(_emissiveMaterial);
  assert(uSampler);

  // emissiveMaterial muss mit angepassten Farb- und Intensitätswerten 
  // ausgestattet werden, da bei globaler Beleuchtung nicht mehr die Fläche-
  // sondern Hemisphärform der LTE verwendet wird
  const_cast<Emissive*>(_emissiveMaterial)->setGlobIlIntensity
    (_ka / _lightShape->getArea() );
  const_cast<Emissive*>(_emissiveMaterial)->setGlobIlColor(_ca);

  _lightShape->setUnitSampler(uSampler);
  _lightShape->setMaterial(_emissiveMaterial);
  _lightShape->setThrowsShadow(false);
  if (showLightShape == true)
    world.addShape(_lightShape);

  /*_lightShape->resetSampler();
  for (size_t i=0; i<_lightShape->numSamples(); i++)
    _samplePoints.push_back(_lightShape->getSample() );*/

}


//_____________________________________________________________________________
AreaLight::~AreaLight(void)
{
}


//_____________________________________________________________________________
Normal
AreaLight::getDirection(const ShadeRec& shadeRec) const
{
  return Normal(shadeRec.lightSamplePnt - shadeRec.hitPoint);
}


//_____________________________________________________________________________
Spectrum
AreaLight::L(const ShadeRec& shadeRec)
{
  Normal lightNormal = _lightShape->getNormal(shadeRec);
  float ndotd = static_cast<float>((-1 * lightNormal) * 
                getDirection(shadeRec) );

  if (ndotd > 0.0)
    return _ka * _ca;

  else
    return BLACK;

}


//_____________________________________________________________________________
bool
AreaLight::inShadow(const Ray& ray, const ShadeRec& sr) const
{
  double t;
  double distance = (ray.o() - sr.lightSamplePnt).length();

  if (_castsShadow == false)
    return false;

  if (sr.world.accelerator != NULL)
  {
    if (sr.world.accelerator->shadowHit(ray, t) && t <distance)
      return true;
  }
  else
  {
    for (size_t i=0; i<sr.world.shapes.size(); i++)
    {
      if (sr.world.shapes[i]->shadowHit(ray, t) &&
          t < distance)
        return true;
    }
  }
  return false;
}


//_____________________________________________________________________________
float
AreaLight::G(const ShadeRec& sr) const
{
  float ndotd = static_cast<float>(-1 * _lightShape->getNormal(sr) * 
                getDirection(sr) );
  
  float dSquared = static_cast<float>((sr.lightSamplePnt - sr.hitPoint).length2() );

  return ndotd / dSquared;
}


//_____________________________________________________________________________
float
AreaLight::pdf(const ShadeRec& sr) const
{
  return _lightShape->pdf(sr);
}


//_____________________________________________________________________________
size_t
AreaLight::numSamples() const
{
  //return _samplePoints.size();
  return _lightShape->numSamples();
}


//_____________________________________________________________________________
std::vector<UPTPhoton >
AreaLight::getStartPhotons(
  const Point& samplePoint, PhotonPool& photonPool,
  HemisphereS* hemisS, uint nPattern/*=100*/) const
{
  std::vector<UPTPhoton > tmp;
  ShadeRec* dummy = nullptr;
  // w = In Richtung zur Grundfläche parallelen Tangentialfläche
  Normal& w = _lightShape->getNormal(*dummy);
  Vector u(-1.8473, 0.3183, 2.94729), v;
  

  orthNormBasis(w, u, v);
  const std::vector<const Point>* samplePoints =
    SStorage::getHSphSamples(hemisS, nPattern);
  tmp.reserve(samplePoints->size() );
  for (auto& it=samplePoints->cbegin(); it!=samplePoints->cend(); it++)
  {
    UPTPhoton photon = photonPool.pop();
    Vector d = u* (*it).x + v* (*it).y + w* (*it).z;
    photon->onlySpecularRefl = true;
    photon->photonRay = Ray(samplePoint, d);
    photon->color     = _ca;
    photon->intensity = _pka;
    tmp.push_back(std::move(photon) );
  }


  return tmp;
}


//_____________________________________________________________________________
//void
//AreaLight::resetSampler()
//{
//  _lightShape->resetSampler();
//}


//_____________________________________________________________________________
//Point
//AreaLight::getSamplePoint(const ShadeRec& sr)
//{
//  _samplePoint = _lightShape->getSample();
//  _lightNormal = _lightShape->getNormal(sr);
//  _wi = (_samplePoint - sr.hitPoint).norm();
//
//  return _samplePoint;
//}


//_____________________________________________________________________________
const std::vector<const Point>&
AreaLight::getSamplePoints(const ShadeRec& sr) const
{
  //assert(_samplePoints.size() != 0);

  return *SStorage::getALightSamples(_lightShape);
  //return _samplePoints;
}