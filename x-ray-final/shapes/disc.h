/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 368 ff.
*/
#pragma once

#ifndef DISC_H_
#define DISC_H_


#include "lightshape.h"
class Disc : public LightShape
{
public:
  Disc(void);
  Disc(const Point& midPoint, float radius,
       const Normal& normal = Normal(0,0,1) );
  virtual ~Disc(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual Point getSample();
  virtual float pdf(const ShadeRec& sr) const;
  virtual Normal getNormal(const ShadeRec& sr) const;
  virtual float getArea() const;


protected:

  virtual void createBV();
  void uSquareToUDisc(float sqX, float sqY, float& diX, float& diY);

  Point _midPoint;
  float _radius;
  Normal _normal;
};




#endif //DISC_H_