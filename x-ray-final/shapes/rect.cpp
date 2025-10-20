#include "stdafx.h"

//#include <algorithm>
//
//#include "rect.h"
//
//#include "aabb.h"
//#include "constants.h"


//_____________________________________________________________________________
Rect::Rect(void)
  : LightShape(), _lowLeftCorner(Point(0,0,0) ), _edge1(Vector(1,0,0) ),
    _edge2(Vector(0,1,0) ), _normal(Normal(0,0,1) )
{
  assert(equals(_normal.length(), 1.) );
  assert(cross(_edge1, _edge2).norm() == _normal);
  assert(equals(_edge1 * _edge2, 0.) );

  _invArea = static_cast<float>(_edge1.length() * _edge2.length() );

  createBV();
}


//_____________________________________________________________________________
Rect::Rect(const Point& lowLeftCorner, const Vector& edge1, const Vector& edge2,
           const Normal& normal)
 : LightShape(), _lowLeftCorner(lowLeftCorner), _edge1(edge1), _edge2(edge2), 
   _normal(normal)
{
  assert(equals(_normal.length(), 1.) );
  assert(cross(_edge1, _edge2).norm() == _normal);
  assert(equals(_edge1 * _edge2, 0.) );

  _invArea = static_cast<float>(_edge1.length() * _edge2.length() );

  createBV();
}


//_____________________________________________________________________________
Rect::~Rect(void)
{
  delete _bv;
  _bv = 0;
}


//_____________________________________________________________________________
bool
Rect::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  double t = (_lowLeftCorner - ray.o() ) * _normal / (ray.d() * _normal);

  if (t <= KEPSILON)
    return false;

  Point p = ray.o() + t * ray.d();
  Vector d = p - _lowLeftCorner;

  double projEdge1 = d * _edge1;
  if (projEdge1 < 0.0 || projEdge1 > _edge1.length2() )
    return false;

  double projEdge2 = d * _edge2;
  if (projEdge2 < 0.0 || projEdge2 > _edge2.length2() )
    return false;

  tmin = t;
  sr.normal = _normal;
  sr.hitPoint = p;
  sr.localHitPoint = p;
  sr.material = _material;

  return true;
}


//_____________________________________________________________________________
bool
Rect::shadowHit(const Ray& ray, double& tmin) const
{
  double t = (_lowLeftCorner - ray.o() ) * _normal / (ray.d() * _normal);

  if (t <= KEPSILON)
    return false;

  Point p = ray.o() + t * ray.d();
  Vector d = p - _lowLeftCorner;

  double projEdge1 = d * _edge1;
  if (projEdge1 < 0.0 || projEdge1 > _edge1.length2() )
    return false;

  double projEdge2 = d * _edge2;
  if (projEdge2 < 0.0 || projEdge2 > _edge2.length2() )
    return false;

  tmin = t;
  return true;
}


//_____________________________________________________________________________
Point
Rect::getSample()
{
  double x, y;
  _uSampler->createSample(x, y);

  return _lowLeftCorner + _edge1 * x + _edge2 * y;
}


//_____________________________________________________________________________
float
Rect::pdf(const ShadeRec& sr) const
{
  return _invArea;
}


//_____________________________________________________________________________
Normal
Rect::getNormal(const ShadeRec& sr) const
{
  return _normal;
}


//_____________________________________________________________________________
float
Rect::getArea() const
{
  return static_cast<float>(_edge1.length() * _edge2.length() );
}


//_____________________________________________________________________________
void
Rect::createBV()
{
  _bv = new AABB(static_cast<float>(std::min(_lowLeftCorner.x, _lowLeftCorner.x
                                             + _edge1.x + _edge2.x) ) - DELTA,
                 static_cast<float>(std::max(_lowLeftCorner.x, _lowLeftCorner.x
                                             + _edge1.x + _edge2.x) ) + DELTA,
                 static_cast<float>(std::min(_lowLeftCorner.y, _lowLeftCorner.y
                                             + _edge1.y + _edge2.y) ) - DELTA,
                 static_cast<float>(std::max(_lowLeftCorner.y, _lowLeftCorner.y
                                             + _edge1.y + _edge2.y) ) + DELTA,
                 static_cast<float>(std::min(_lowLeftCorner.z, _lowLeftCorner.z
                                             + _edge1.z + _edge2.z) ) - DELTA,
                 static_cast<float>(std::max(_lowLeftCorner.z, _lowLeftCorner.z
                                             + _edge1.z + _edge2.z) ) + DELTA );
}