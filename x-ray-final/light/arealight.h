/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 331ff.
*/


#pragma once

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include <memory>

#include "emissive.h"
#include "light.h"
#include "lightshape.h"
#include "photonpool.h"

/* TODO : Die Positionen von AreaLights hängen von der globalen Postion der 
          LightShapes ab. Es kann also keine Transformation wie für Instanzen
          durchgeführt werden.
*/

// Forward Deklarationen
class HemisphereS;
struct TracePhoton;

typedef std::unique_ptr<const TracePhoton> UPCTPhoton;
typedef std::unique_ptr<TracePhoton> UPTPhoton;


class AreaLight : public Light
{
public:
  //AreaLight(void);
  AreaLight(float intensity, const Spectrum& color,
            LightShape* lightShape, const Emissive* emissiveMaterial,
            UnitSampler* uSampler, World& world, bool showLightShape=true,
            bool castsShadow=true);
  AreaLight(float intensity, float photonIntensity, const Spectrum& color,
            LightShape* lightShape, const Emissive* emissiveMaterial,
            UnitSampler* uSampler, World& world, bool showLightShape=true,
            bool castsShadow=true);
  virtual ~AreaLight(void);

  virtual Normal getDirection(const ShadeRec& shadeRec) const;
  virtual Spectrum L(const ShadeRec& shadeRec);
  virtual bool inShadow(const Ray& ray, const ShadeRec& sr) const;
  virtual Spectrum I() const {return _ca * _ka / _lightShape->getArea();}
  //virtual Point getSamplePoint(const ShadeRec& sr);
  virtual const std::vector<const Point>& getSamplePoints(const ShadeRec& sr) const;
  virtual float G(const ShadeRec& sr) const;
  virtual float pdf(const ShadeRec& sr) const;
  virtual size_t numSamples() const;
  virtual std::vector<UPTPhoton > getStartPhotons(
    const Point& samplePoint, PhotonPool& photonPool,
    HemisphereS* hemisS, uint nPattern=100) const;
  //virtual void resetSampler();

protected:
  float _ka;
  float _pka;
  Spectrum _ca;
  LightShape* _lightShape;
  const Emissive*  _emissiveMaterial;
  //Normal _lightNormal;
  //Vector _wi; //Einheitsvektor vom hitPoint zum samplePoint
  //std::vector<const Point> _samplePoints;


  

};

#endif //AREALIGHT_H_