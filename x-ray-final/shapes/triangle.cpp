#include "stdafx.h"

//#include <algorithm>
//
//#include "aabb.h"
//#include "constants.h"
//#include "triangle.h"



//_____________________________________________________________________________
Triangle::Triangle(void)
  : LightShape(), _p0(Point(-0.5,-sqrt(3.)/4,0) ), 
    _p1(Point(0.5,-sqrt(3.)/4,0) ), _p2(Point(0,sqrt(3.)/4,0) ), 
    _normal(Normal(0,0,1) )
{
  _invArea = 1.f / A(_p0, _p1, _p2);

  createBV();
}


//_____________________________________________________________________________
Triangle::Triangle(Point p0, Point p1, Point p2)
  : LightShape(A(p0,p1,p2) ), _p0(p0), _p1(p1), _p2(p2),
    _normal(cross(p1-p0, p2-p0) )
{

  createBV();
}


//_____________________________________________________________________________
Triangle::~Triangle(void)
{
  delete _bv;
  _bv = 0;
}


//_____________________________________________________________________________
bool
Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  Vector e1 = _p1 - _p0;
  Vector e2 = _p2 - _p0;
  Vector s1  = cross(ray.d(), e2);
  double divisor = s1 * e1;

  if (divisor == 0.)
    return false;
  double invDivisor = 1.f / divisor;

  Vector d = ray.o() - _p0;
  double b1 = d * s1 * invDivisor;
  if (b1 < 0. || b1 > 1.)
    return false;

  Vector s2 = cross(d, e1);
  double b2 = ray.d() * s2 * invDivisor;
  if (b2 < 0. || b1 + b2 > 1.)
    return false;

  double t = e2 * s2 * invDivisor;
  if (t <= KEPSILON)
    return false;

  tmin = t;
  sr.localHitPoint = ray.o() + tmin * ray.d();
  sr.hitPoint = sr.localHitPoint;
  sr.hitAnObject = true;
  sr.material = _material;
  sr.normal = _normal;
  sr.t = tmin;
  sr.ray = ray;  

  return true;
}


//_____________________________________________________________________________
bool
Triangle::shadowHit(const Ray& ray, double& tmin) const
{
 /* Vector e1 = _p1 - _p0;
  Vector e2 = _p2 - _p0;
  Vector s  = ray.o - _p0;

  double det = 1. / (cross(ray.d, e2) * e1);
  double t = det * cross(s, e1) * e2;

  if (tmin < KEPSILON)
    return false;
  
  double b1 = det * cross(ray.d, e2) * s;
  double b2 = det * cross(s, e1) * ray.d;

  if (b1 < 0 || b2 < 0 || b1 + b2 > 1)
    return false;
  
  tmin = t;
  return true;*/
 
  Vector e1 = _p1 - _p0;
  Vector e2 = _p2 - _p0;
  Vector s1  = cross(ray.d(), e2);
  double divisor = s1 * e1;

  if (divisor == 0.)
    return false;
  double invDivisor = 1.f / divisor;

  Vector d = ray.o() - _p0;
  double b1 = d * s1 * invDivisor;
  if (b1 < 0. || b1 > 1.)
    return false;

  Vector s2 = cross(d, e1);
  double b2 = ray.d() * s2 * invDivisor;
  if (b2 < 0. || b1 + b2 > 1.)
    return false;

  double t = e2 * s2 * invDivisor;
  if (t <= KEPSILON)
    return false;

  tmin = t;
  return true;
}


//_____________________________________________________________________________
Point
Triangle::getSample()
{
  // Verwende Baryzentrische Koordinaten um ein Dreieck zu samplen
  double u, v, x, y;

  _uSampler->createSample(u, v);
  
  double tmp = sqrt(u);
  x = 1. - tmp;
  y = v * tmp;

  return Point((1-x-y) * _p0.x + (x * _p1.x) + (y * _p2.x),
               (1-x-y) * _p0.y + (x * _p1.y) + (y * _p2.y),
               (1-x-y) * _p0.z + (x * _p1.z) + (y * _p2.z) );
}


//_____________________________________________________________________________
float
Triangle::pdf(const ShadeRec& sr) const
{
  return _invArea;
}


//_____________________________________________________________________________
Normal
Triangle::getNormal(const ShadeRec& sr) const
{
  return _normal;
}


//_____________________________________________________________________________
float
Triangle::getArea() const
{
  FLOATING a = (_p0 - _p1).length();
  FLOATING b = (_p0 - _p2).length();
  FLOATING c = (_p1 - _p2).length();

  return static_cast<float>(0.25 * std::sqrt((a+b+c) * (a+b-c) * (b+c-a)* 
                                             (c+a-b) ) );
}


//_____________________________________________________________________________
float
Triangle::A(const Point& p0, const Point& p1, const Point& p2)
{
  // Nutzt den Satz des Heron zur Berechnung des Flächeninhalts
  double a = (p0 - p1).length();
  double b = (p0 - p2).length();
  double c = (p1 - p2).length();

  return static_cast<float>(0.25 * std::sqrt((a+b+c) * (a+b-c) * (b+c-a)* 
                                             (c+a-b) ) );
}


//_____________________________________________________________________________
void
Triangle::createBV()
{
  delete _bv;
  _bv = 0;

  _bv = new AABB(static_cast<float>(std::min(std::min(_p0.x, _p1.x), _p2.x) ) -
                 DELTA,
                 static_cast<float>(std::max(std::max(_p0.x, _p1.x), _p2.x) ) +
                 DELTA,
                 static_cast<float>(std::min(std::min(_p0.y, _p1.y), _p2.y) ) -
                 DELTA,
                 static_cast<float>(std::max(std::max(_p0.y, _p1.y), _p2.y) ) +
                 DELTA,
                 static_cast<float>(std::min(std::min(_p0.z, _p1.z), _p2.z) ) -
                 DELTA,
                 static_cast<float>(std::max(std::max(_p0.z, _p1.z), _p2.z) ) +
                 DELTA);
}