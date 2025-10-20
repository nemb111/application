/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Physically Based Rendering" / Seite 140 ff.
*/
#pragma once

#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "lightshape.h"
class Triangle : public LightShape
{
public:
  Triangle(void);
  Triangle(Point p0, Point p1, Point p2);
  virtual ~Triangle(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual Point getSample();
  virtual float pdf(const ShadeRec& sr) const;
  virtual Normal getNormal(const ShadeRec& sr) const;
  virtual float getArea() const;

  virtual const Point& getP0() const {return _p0;}
  virtual const Point& getP1() const {return _p1;}
  virtual const Point& getP2() const {return _p2;}

protected:
  static float A(const Point& p0, const Point& p1, const Point& p2);
  virtual void createBV();
  

  Point _p0;
  Point _p1;
  Point _p2;
  Normal _normal;
};


#endif//TRIANGLE_H_