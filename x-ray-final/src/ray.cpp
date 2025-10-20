#include "stdafx.h"

//#include "ray.h"


//_____________________________________________________________________________
Ray::Ray(void)
  : _o(Point(0, 0, 0) ), _d(Vector(0, 0, -1) )
{

}


//_____________________________________________________________________________
Ray::Ray(const Point& origin, const Vector& direction)
  : _o(origin), _d(direction)
{
  assert(_d.length() <= 1.0001);
}


//_____________________________________________________________________________
Ray::~Ray(void)
{
}


//_____________________________________________________________________________
bool
Ray::operator==(const Ray& rhs) const
{
  return (_o == rhs._o && _d == rhs._d);
}


//_____________________________________________________________________________
InvRay::InvRay(const Point& origin, const Vector& direction)
{
  Ray::_o = origin;
  Ray::_d = direction;
}


//_____________________________________________________________________________
ShadeRec::ShadeRec(const World& wr) 
  : hitAnObject(false), material(NULL), hitPoint(Point()), 
    localHitPoint(Point()), normal(Normal()), ray(Ray()), depth(0), t(0.),
    transMat(HMatrix::Identity() ), invTransMat(HMatrix::Identity() ),
    contribution(1.f), world(wr)/*, spectrum(BLACK)*/
{

}


//_____________________________________________________________________________
ShadeRec&
ShadeRec::operator=(const ShadeRec& rhs)
{

  hitAnObject = rhs.hitAnObject;
  material = rhs.material;
  hitPoint = rhs.hitPoint;
  localHitPoint = rhs.localHitPoint;
  normal = rhs.normal;
  uv = rhs.uv;
  ray = rhs.ray;
  depth = rhs.depth;
  t = rhs.t;
  //spectrum = rhs.spectrum;

  return *this;
}