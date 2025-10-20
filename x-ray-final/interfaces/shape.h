/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 53ff.
*/


#pragma once

#ifndef SHAPE_H_
#define SHAPE_H_

#include "bv.h"
#include "material.h"
#include "ray.h"

//Forward Deklarationen
class BV;
class Material;
class Ray;
class ShadeRec;



class Shape
{
public:
  Shape(const stdstring& name=_T(""), bool throwsShadow=true);
  virtual ~Shape(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
  virtual bool shadowHit(const Ray& ray, double& tmin) const = 0;
  virtual const BV& getBV() const;
  virtual void setMaterial(const Material* material);
  virtual void setThrowsShadow(bool throwsShadow);
  virtual bool getThrowsShadow() const {return _throwsShadow;}
  virtual stdstring getName() const {return _name;}
  virtual void setName(const stdstring& name) {_name = name;}
 

protected:

  virtual void createBV() = 0;

  stdstring _name;
  bool _matset;
  bool _throwsShadow;
  const Material* _material;
  const BV* _bv;
  
};

#endif//SHAPE_H_