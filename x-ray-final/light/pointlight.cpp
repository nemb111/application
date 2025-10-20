#include "stdafx.h"


//#include "pointlight.h"


//_____________________________________________________________________________
PointLight::PointLight(void)
  : Light(), _position(Point(0,0,0) ), _color(WHITE), _intensity(1.), 
    _dsphere(NULL), _dmaterial(NULL), _instance(NULL)
{
}


//_____________________________________________________________________________
PointLight::PointLight(bool castsShadow)
  : Light(castsShadow), _position(Point(0,0,0) ), _color(WHITE), _intensity(1.),
    _dsphere(NULL), _dmaterial(NULL), _instance(NULL)
{
}


//_____________________________________________________________________________
PointLight::PointLight(const Point& position, const Spectrum& color/*=WHITE*/,
                       float intensity/*=1.*/, bool castsShadow/*=true*/)
  : Light(castsShadow), _position(position), _color(color),
    _intensity(intensity), _dsphere(NULL), _dmaterial(NULL), _instance(NULL)
{
}


//_____________________________________________________________________________
PointLight::~PointLight(void)
{
  delete _dsphere;
  _dsphere = 0;
  delete _dmaterial;
  _dmaterial = 0;
  delete _instance;
  _instance = 0;
}


//_____________________________________________________________________________
Normal
PointLight::getDirection(const ShadeRec& shadeRec) const
{
  return Normal(_position - shadeRec.hitPoint);
}

//_____________________________________________________________________________
Spectrum 
PointLight::L(const ShadeRec& shadeRec)
{
  return _intensity * _color;
}


//_____________________________________________________________________________
bool
PointLight::inShadow(const Ray& ray, const ShadeRec& sr) const
{
  double t;
  double distance = (ray.o() - _position).length();

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
void 
PointLight::showSphere(World& world)
{
  if (_dsphere || _dmaterial || _instance)
  {
    WOUT("Die Methode \"showSphere\" wurde bereits verwendet."
         "Nichts geschieht.");
    return;
  }

  _dsphere = new DebugSphere(_position, 0.1);
  _dmaterial = new DebugMaterial(WHITE);
  _dsphere->setMaterial(_dmaterial);
  _instance = new Instance(_dsphere);
  _instance->setThrowsShadow(false);

  world.addShape(_instance);
}


//_____________________________________________________________________________
void
PointLight::showSphere(World& world, DebugSphere* dsphere,
                       const Spectrum& color)
{
  if (_dsphere || _dmaterial || _instance)
  {
    WOUT("Die Methode \"showSphere\" wurde bereits verwendet."
         "Nichts geschieht.");
    return;
  }

  _dmaterial = new DebugMaterial(color);
  _dsphere = dsphere;
  _dsphere->setMaterial(_dmaterial);
  _instance = new Instance(_dsphere);
  _instance->setThrowsShadow(false);
  _instance->translate(_position.x, _position.y, _position.z);


  world.addShape(_instance);
}