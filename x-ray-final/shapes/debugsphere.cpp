#include "stdafx.h"

//#include "debugsphere.h"


DebugSphere::DebugSphere(void)
  : Sphere()
{
  // Nicht schön gelöst. Im Moment zu faul den Konstruktor umzuschreiben
  setThrowsShadow(false);
}


DebugSphere::DebugSphere(const Point& center, double radius)
  : Sphere(center, radius)
{
  // Nicht schön gelöst. Im Moment zu faul den Konstruktor umzuschreiben
  setThrowsShadow(false);
}


DebugSphere::~DebugSphere(void)
{
}


bool
DebugSphere::shadowHit(const Ray& ray, double& tmin) const
{
  return false;
}


void
DebugSphere::setMaterial(Material* material)
{
  Sphere::setMaterial(material);
}