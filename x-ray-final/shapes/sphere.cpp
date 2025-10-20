#include "stdafx.h"

//#include "sphere.h"
//
//#include "aabb.h"
//#include "constants.h"

//_____________________________________________________________________________
Sphere::Sphere(void)
  : center(Point(0, 0, 0) ), radius(1.)
{
  createBV();
}


//_____________________________________________________________________________
Sphere::Sphere(const Point& center, double radius)
  : center(center), radius(radius)
{
  createBV();
}

//_____________________________________________________________________________
Sphere::~Sphere(void)
{
  delete _bv;
  _bv = 0;
}


//_____________________________________________________________________________
bool
Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  double t;
  Vector tmp  = ray.o() - center;
  double a    = ray.d() * ray.d();
  double b    = 2.0 * tmp * ray.d();
  double c    = tmp * tmp - radius * radius;
  double discriminant = b * b - 4.0 * a * c;

  if (discriminant < 0.0)
    return false;
  else
  {
    double e = sqrt(discriminant);
    double denom = 2.0 * a;
    t = (-b - e) / denom; //smaller root

    if (t > KEPSILON)
    {
      tmin = t;
      sr.t = t;
      sr.hitAnObject = true;
      sr.normal = (tmp + t * ray.d() ) * (1./radius);
      sr.hitPoint = ray.o() + t * ray.d();
      sr.localHitPoint = ray.o() + t * ray.d();
      sr.material = _material;
      return true;
    }

    t = (-b + e) /denom; //larger root
    if (t > KEPSILON)
    {
      tmin = t;
      sr.t = t;
      sr.hitAnObject = true;
      sr.normal = (tmp + t * ray.d() ) * (1./radius);
      sr.hitPoint = ray.o() + t * ray.d();
      sr.localHitPoint = ray.o() + t * ray.d();
      sr.material = _material;
      return true;
    }
  }

  return false;
}


//_____________________________________________________________________________
bool
Sphere::shadowHit(const Ray& ray, double& tmin) const
{
  double t;
  Vector tmp  = ray.o() - center;
  double a    = ray.d() * ray.d();
  double b    = 2.0 * tmp * ray.d();
  double c    = tmp * tmp - radius * radius;
  double discriminant = b * b - 4.0 * a * c;

  if (discriminant < 0.0)
    return false;
  else
  {
    double e = sqrt(discriminant);
    double denom = 2.0 * a;
    t = (-b - e) / denom; //smaller root

    if (t > KEPSILON)
    {
      tmin = t;
      return true;
    }

    t = (-b + e) /denom; //larger root
    if (t > KEPSILON)
    {
      tmin = t;
      return true;
    }
  }

  return false;
}


//_____________________________________________________________________________
void
Sphere::createBV()
{
  _bv = new AABB(static_cast<float>(center.x - radius),
                 static_cast<float>(center.x + radius),
                 static_cast<float>(center.y - radius),
                 static_cast<float>(center.y + radius),
                 static_cast<float>(center.z - radius),
                 static_cast<float>(center.z + radius) );
}