/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 54 ff.
*/
#pragma once

#ifndef PLANE_H_
#define PLANE_H_



#include "shape.h"
class Plane : public Shape
{
public:
  Plane(void);
  Plane(const Point& p, const Normal& normal);
  virtual ~Plane(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;


protected:

  virtual void createBV();


  Point _p;
  Normal _normal;
};

#endif//PLANE_H_