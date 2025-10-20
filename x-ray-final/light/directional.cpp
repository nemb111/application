#include "stdafx.h"

//#include "directional.h"


//_____________________________________________________________________________
Directional::Directional(void)
  : Light(), _color(WHITE), _direction(Vector(0,1,0) ), _intensity(1.)
{
}


//_____________________________________________________________________________
Directional::Directional(bool castsShadow)
  : Light(castsShadow), _color(WHITE), _direction(Vector(0,1,0) ),
    _intensity(1.)
{
}


//_____________________________________________________________________________
Directional::Directional(const Spectrum& color, const Vector& direction,
                         float intensity/*=1.*/, bool castsShadow/*=true*/)
  : Light(castsShadow), _color(color), _direction(-1 * direction ),
    _intensity(intensity)
{
}

//_____________________________________________________________________________
Directional::~Directional(void)
{
}


//_____________________________________________________________________________
Normal
Directional::getDirection(const ShadeRec& shadeRec) const
{
  return _direction;
}


//_____________________________________________________________________________
Spectrum
Directional::L(const ShadeRec& shadeRec)
{
  return _color * _intensity;
}


//_____________________________________________________________________________
bool
Directional::inShadow(const Ray& ray, const ShadeRec& sr) const
{
  assert (sr.world.accelerator != NULL);

  if (_castsShadow == false)
    return false;

  double t;

  //if (sr.world.accelerator != NULL)
  //{
  if (sr.world.accelerator->shadowHit(ray, t) )
    return true;
  /*}
  else
  {
    for (size_t i=0; i<sr.world.shapes.size(); i++)
    {
      if (sr.world.shapes[i]->shadowHit(ray, t) )
        return true;
    }
  }*/

  return false;
}