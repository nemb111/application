
#pragma once

#ifndef ACCELERATOR_H_
#define ACCELERATOR_H_

#include <vector>

#include "ray.h"
#include "shape.h"

//Forward Deklarationen
class Ray;
class Shape;

class Accelerator
{
public:
  Accelerator(void);
  Accelerator(const std::vector<Shape*>& shapes);
  
  virtual ~Accelerator(void);

  virtual void addShapes(const std::vector<Shape*>& shapes);
  virtual Accelerator* newInstance() const = 0;
  virtual bool create() = 0;
  virtual Accelerator* clone() const = 0;

  virtual bool hit(const Ray& ray, size_t& shpIdx, double* tmin=NULL) const = 0;
  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
  virtual bool shadowHit(const Ray& ray, double& tmin) const = 0;

protected:

  const std::vector<Shape*>* _shapesRef;

private:
  
};

#endif //ACCELERATOR_H_