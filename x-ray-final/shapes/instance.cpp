#include "stdafx.h"

//#include "instance.h"
//
//
//#include "linAlg.h"

//_____________________________________________________________________________
//Instance::Instance(void)
//{
//}


//_____________________________________________________________________________
Instance::Instance(const Shape* shape)
  : _shape(shape), _globalBV(NULL)
{
  assert(shape != NULL);
  assert(dynamic_cast<const Instance*>(shape) == 0);
  _transformTheTexture = false;
}


//_____________________________________________________________________________
Instance::~Instance(void)
{
  delete _globalBV;
  _globalBV = 0;
}


//_____________________________________________________________________________
bool 
Instance::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  InvRay invRay(_invMatrix * ray.o(), _invMatrix * ray.d() );

  if (_shape->hit(invRay, tmin, sr))
  {
    sr.normal = _invMatrix.transpose() * sr.normal;
    sr.hitAnObject = true;
    sr.hitPoint = ray.o() + tmin * ray.d();
    sr.transMat = _matrix;
    sr.invTransMat = _invMatrix;
    return true;
  }
  else
    return false;


}


//_____________________________________________________________________________
bool 
Instance::shadowHit(const Ray& ray, double& tmin) const
{
  InvRay invRay(_invMatrix * ray.o(), _invMatrix * ray.d() );

  if (_shape->shadowHit(invRay, tmin))
    return true;
  else
    return false;


}


//_____________________________________________________________________________
const BV&
Instance::getBV() const
{
  if (_globalBV)
    return *_globalBV;

  const AABB& aabb = dynamic_cast<const AABB&>(_shape->getBV() );

  Point p0(aabb.minSlab[0], aabb.minSlab[1], aabb.minSlab[2]);
  Point p1(aabb.maxSlab[0], aabb.minSlab[1], aabb.minSlab[2]);
  Point p2(aabb.maxSlab[0], aabb.maxSlab[1], aabb.minSlab[2]);
  Point p3(aabb.minSlab[0], aabb.maxSlab[1], aabb.minSlab[2]);
  Point p4(aabb.minSlab[0], aabb.minSlab[1], aabb.maxSlab[2]);
  Point p5(aabb.maxSlab[0], aabb.minSlab[1], aabb.maxSlab[2]);
  Point p6(aabb.maxSlab[0], aabb.maxSlab[1], aabb.maxSlab[2]);
  Point p7(aabb.minSlab[0], aabb.maxSlab[1], aabb.maxSlab[2]);

  //Transformiere die Punkte der lokalen AABB um eine globale 
  //(evt passungenauere) AABB zu erzeugen
  p0 = _matrix * p0;
  p1 = _matrix * p1;
  p2 = _matrix * p2;
  p3 = _matrix * p3;
  p4 = _matrix * p4;
  p5 = _matrix * p5;
  p6 = _matrix * p6;
  p7 = _matrix * p7;

  _globalBV = new AABB();

  _globalBV->extend(p0);
  _globalBV->extend(p1);
  _globalBV->extend(p2);
  _globalBV->extend(p3);
  _globalBV->extend(p4);
  _globalBV->extend(p5);
  _globalBV->extend(p6);
  _globalBV->extend(p7);

  return *_globalBV;
}


//_____________________________________________________________________________
void
Instance::setMaterial(const Material* material)
{
  WOUT("Die Instance Klasse wird hauptsächlich verwendet um "
       "geometrische Objekte zu transformieren. Das Material sollte deshalb "
       "in den geom. Objekten gesetzt werden. Der Aufruf dieser Methode "
       "bleibt ohne Effekt!");
}


//_____________________________________________________________________________
void
Instance::setThrowsShadow(bool throwsShadow)
{
  WOUT("Die Instance Klasse wird hauptsächlich verwendet um "
       "geometrische Objekte zu transformieren. Das Schattenwurfattribut"
       "sollte deshalb "
       "in den geom. Objekten gesetzt werden. Der Aufruf dieser Methode "
       "bleibt ohne Effekt!");
}


//_____________________________________________________________________________
bool
Instance::getThrowsShadow() const
{
  return _shape->getThrowsShadow();
}


//_____________________________________________________________________________
stdstring
Instance::getName() const
{
  return _shape->getName();
}


//_____________________________________________________________________________
void
Instance::setName(const stdstring& name)
{
  WOUT("Die Instance Klasse wird hauptsächlich verwendet um "
       "geometrische Objekte zu transformieren. Der Name sollte deshalb "
       "in den geom. Objekten gesetzt werden. Der Aufruf dieser Methode "
       "bleibt ohne Effekt!");
}


//_____________________________________________________________________________
void
Instance::translate(double tx, double ty, double tz)
{
  _matrix    = TransMat(tx, ty, tz) * _matrix;
  _invMatrix = _invMatrix * TransMat(-tx, -ty, -tz);
}


//_____________________________________________________________________________
void
Instance::rotateX(double phi)
{
  _matrix    = RotMatX(phi) * _matrix;
  _invMatrix = _invMatrix * RotMatX(-phi);
}


//_____________________________________________________________________________
void
Instance::rotateY(double phi)
{
  _matrix    = RotMatY(phi) * _matrix;
  _invMatrix = _invMatrix * RotMatY(-phi);
}


//_____________________________________________________________________________
void
Instance::rotateZ(double phi)
{
  _matrix    = RotMatZ(phi) * _matrix;
  _invMatrix = _invMatrix * RotMatZ(-phi);
}


//_____________________________________________________________________________
void
Instance::scale(double sx, double sy, double sz)
{
  _matrix    = ScaleMat(sx, sy, sz) * _matrix;
  _invMatrix = _invMatrix * ScaleMat(1./sx, 1./sy, 1./sz);
}


//_____________________________________________________________________________
void
Instance::reflectX()
{
  _matrix    = ReflMatX() * _matrix;
  _invMatrix = _invMatrix * ReflMatX();
}


//_____________________________________________________________________________
void
Instance::reflectY()
{
  _matrix    = ReflMatY() * _matrix;
  _invMatrix = _invMatrix * ReflMatY();
}


//_____________________________________________________________________________
void
Instance::reflectZ()
{
  _matrix    = ReflMatZ() * _matrix;
  _invMatrix = _invMatrix * ReflMatZ();
}


//_____________________________________________________________________________
void
Instance::setInvTransformation(const HMatrix& invHMatrix)
{
  _invMatrix = invHMatrix;
}


//_____________________________________________________________________________
void
Instance::setTransformation(const HMatrix& hMatrix)
{
  _matrix = hMatrix;
}