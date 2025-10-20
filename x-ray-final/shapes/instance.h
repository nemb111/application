#pragma once

#ifndef INSTANCE_H_ 
#define INSTANCE_H_

#include "aabb.h"
#include "ray.h"
#include "shape.h"

class Instance : public Shape
{
public:
  Instance(const Shape* shape);
  virtual ~Instance(void);

    virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
    virtual bool shadowHit(const Ray& ray, double& tmin) const;
    virtual const BV& getBV() const;
    virtual void setMaterial(const Material* material);
    virtual void setThrowsShadow(bool throwsShadow);
    virtual bool getThrowsShadow() const;
    virtual stdstring getName() const;
    virtual void setName(const stdstring& name);
    virtual const Shape* getContent() const {return _shape;} 


  //Funktionen für Transformationen
  virtual void translate(double tx, double ty, double tz);
  virtual void rotateX(double phi);
  virtual void rotateY(double phi);
  virtual void rotateZ(double phi);
  virtual void scale(double sx, double sy, double sz);
  virtual void reflectX();
  virtual void reflectY();
  virtual void reflectZ();
  //Methoden die beliebige Transformationen erlaubt. Es wird nicht auf
  //unerlaubte Transformationen getestet.
  virtual void setTransformation(const HMatrix& hMatrix);
  virtual void setInvTransformation(const HMatrix& invHMatrix);


protected:
  //Instance(void);

  virtual void createBV(){}

  const Shape* _shape;
  HMatrix _matrix;    //Transformationsmatrix
  HMatrix _invMatrix; //inverse Transformationsmatrix
  bool _transformTheTexture;


  mutable AABB* _globalBV;

#ifdef _DEBUG
  friend class TestInstance;
#endif//_DEBUG
};

#endif//INSTANCE_H_