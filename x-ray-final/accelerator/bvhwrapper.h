#pragma once

#ifndef BVHWRAPPER_H_
#define BVHWRAPPER_H_

#include "accelerator.h"
#include "bvh/BVH.h"
#include "bvh/Object.h"
#include "bvh/Vector3.h"
#include "shape.h"



class ObjectWrapper : public BVHNS::Object
{

public:
  ObjectWrapper(const Shape& shape, size_t idx) : _idx(idx), _shape(shape) {}
  ~ObjectWrapper(){}

  virtual bool getIntersection(const BVHNS::Ray& ray, 
                               BVHNS::IntersectionInfo* intersection) const; 
  virtual BVHNS::Vector3 getNormal(const BVHNS::IntersectionInfo& I) const;
  virtual BVHNS::BBox getBBox() const;
  virtual BVHNS::Vector3 getCentroid() const;
  size_t getIdx() const {return _idx;}

private:

  const Shape& getShape() {return _shape;}

  size_t _idx;
  const Shape& _shape;

};



class BVHwrapper : public Accelerator
{
public:
  BVHwrapper(void);
  BVHwrapper(const std::vector<Shape*>& shapes);
  virtual ~BVHwrapper(void);

  virtual Accelerator* newInstance() const;
  virtual bool create();
  virtual Accelerator* clone() const;

  virtual bool hit(const Ray& ray, size_t& shpIdx, double* tmin=NULL) const;
  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

private:
  bool _isInit;
  BVHNS::BVH* _bvh;
  BVHNS::BVH* _shadowBVH;
  std::vector<BVHNS::Object*> _objects;
  std::vector<BVHNS::Object*> _shadowObjects;
};


#endif //BVHWRAPPER_H_