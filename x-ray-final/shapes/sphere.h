/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 58ff.
*/

#pragma once

#ifndef SPHERE_H_
#define SPHERE_H_


#include "shape.h"
class Sphere : public Shape
{
public:
  Sphere(void);
  Sphere(const Point& center, double radius);
  virtual ~Sphere(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  Point  center;
  double radius;

protected:
  virtual void createBV();
};

#endif//SHPERE_H_