#pragma once

#ifndef DEBUGSPHERE_H_
#define DEBUGSPHERE_H_

#include "sphere.h"
class DebugSphere : public Sphere
{
public:
  DebugSphere(void);
  DebugSphere(const Point& center, double radius);
  virtual ~DebugSphere(void);

  virtual bool shadowHit(const Ray& ray, double& tmin) const;

protected:
  virtual void setMaterial(Material* material);



  friend class PointLight;
};

#endif//DEBUGSPHERE_H_