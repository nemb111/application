#pragma once

#ifndef NOACCEL_H_
#define NOACCEL_H_

#include "accelerator.h"

class NoAccel : public Accelerator
{
public:
  NoAccel(void);
  NoAccel(const std::vector<Shape*>& shapes);
  virtual ~NoAccel(void);

  virtual Accelerator* newInstance() const;
  virtual bool create();
  virtual Accelerator* clone() const;

  virtual bool hit(const Ray& ray, size_t& shpIdx, double* tmin=NULL) const;
  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;


};

#endif //NOACCEL_H_