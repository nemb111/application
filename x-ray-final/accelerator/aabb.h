/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Physically based rendering" / Seite 195
*/

#pragma once

#ifndef AABB_H_
#define AABB_H_


#include "bv.h"
class AABB : public BV
{
public:
	AABB();
  AABB(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	virtual ~AABB();

  static AABB empty();

	virtual bool hit(const Ray& ray) const;
  virtual bool hit(const Ray& ray, float& tmin, float& tmax) const;

  void extend(const Point& point);
  void extend(const AABB& aabb);
  float getArea() const;
  Point getCentroid() const;
  Vector diagonal() const;


	float __declspec(align(16)) minSlab[4];
  float __declspec(align(16)) maxSlab[4];

  protected:

  private:
};

#endif //AABB_H_