/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 369 ff.
*/
#pragma once

#ifndef RECT_H_
#define RECT_H_

#include "lightshape.h"
class Rect : public LightShape
{
public:
  Rect(void);
  Rect(const Point& lowLeftCorner, const Vector& edge1, const Vector& edge2,
       const Normal& normal);
  virtual ~Rect(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual Point getSample();
  virtual float pdf(const ShadeRec& sr) const;
  virtual Normal getNormal(const ShadeRec& sr) const;
  virtual float getArea() const;


protected:

  virtual void createBV();


  Point _lowLeftCorner;
  Vector _edge1;
  Vector _edge2;
  Normal _normal;
};

#endif//RECT_H_