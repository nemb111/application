#include "stdafx.h"


//#include "constants.h"
//#include "plane.h"

//_____________________________________________________________________________
Plane::Plane(void)
  : _p(Point(0,0,0) ), _normal(Normal(0,1,0) )
{
}


//_____________________________________________________________________________
Plane::Plane(const Point& p, const Normal& normal)
  : _p(p), _normal(normal)
{

}


//_____________________________________________________________________________
Plane::~Plane(void)
{
}


//_____________________________________________________________________________
bool
Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  double t = (_p - ray.o() ) * _normal / (ray.d() * _normal);

  if (t < KEPSILON)
    return false;


  tmin = t;
  sr.t = t;
  sr.normal = _normal;
  sr.localHitPoint = ray.o() + ray.d() * t;
  sr.material = _material;
  sr.hitAnObject = true;

  return true;

}


//_____________________________________________________________________________
bool
 Plane::shadowHit(const Ray& ray, double& tmin) const
{
  double t = (_p - ray.o() ) * _normal / (ray.d() * _normal);

  if (t < KEPSILON)
    return false;
  
  tmin = t;
  return true;
}