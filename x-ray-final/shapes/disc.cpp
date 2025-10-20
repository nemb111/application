#include "stdafx.h"


//#include <cmath>
//
//#include "disc.h"
//
//#include "aabb.h"
//#include "constants.h"


//_____________________________________________________________________________
Disc::Disc(void)
  : LightShape(static_cast<float>(PI) ), _midPoint(Point(0,0,0) ),
    _radius(1.f), _normal(Normal(0,0,1) )
{
  createBV();
}


//_____________________________________________________________________________
Disc::Disc(const Point& midPoint, float radius,
           const Normal& normal/*= Normal(0,0,1)*/ )
 : LightShape(static_cast<float>(PI * radius * radius) ), _midPoint(midPoint),
   _radius(radius),
   _normal(normal)
{
  assert(_radius > 0 );

  createBV();
}


//_____________________________________________________________________________
Disc::~Disc(void)
{
  delete _bv;
  _bv = 0;
}


//_____________________________________________________________________________
bool
Disc::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  double t = (_midPoint - ray.o() ) * _normal / (ray.d() * _normal);

  if (t <= KEPSILON)
    return false;

  Point hitPoint = ray.o() + t * ray.d();
  if ( (hitPoint - _midPoint).length() > _radius)
    return false;
  
  tmin = t;
  sr.normal = _normal;
  sr.hitPoint = hitPoint;
  sr.localHitPoint = hitPoint;
  sr.material = _material;

  return true;
}


//_____________________________________________________________________________
bool
Disc::shadowHit(const Ray& ray, double& tmin) const
{
  double t = (_midPoint - ray.o() ) * _normal / (ray.d() * _normal);

  if (t <= KEPSILON)
    return false;

  if ( ((ray.o() + t * ray.d()) - _midPoint).length() <= _radius)
  {
    tmin = t;

    return true;
  }
  else
    return false;
}


//_____________________________________________________________________________
void
Disc::createBV()
{
  //float xmin, xmax, ymin, ymax, zmin, zmax;
  //double phiY, phiRotVec;
  //Normal e0(0,1,0), e1(1,0,0);
  //Normal xzProj(_normal.x, 0, _normal.z), rotVec;
  //Point p0, p1, p2, p3;

  //HMatrix transMat = HMatrix::Identity();

 
  //if (xzProj == Vector(0,0,0) )
  //{
  //  xzProj = Normal(0,0,1);
  //  phiY = 0.;
  //}
  //else
  //  phiY = angle(xzProj, Normal(0,0,1) );

  //transMat.transf(RotMatY(phiY) );
  //e1 = transMat * e1;

  //phiRotVec = angle(xzProj, _normal );
  //rotVec = cross(xzProj, _normal);
  //
  //if (rotVec == Normal(0,0,0) )
  //{
  //  rotVec = Normal(0,0,1);
  //  phiRotVec = 0.;
  //}

  //RotMat rotMat(rotVec, phiRotVec);
  //e0 = rotMat * e0;
  //e1 = rotMat * e1;

  //p0 = e0 * _radius + _midPoint;
  //p1 = e0 * (-_radius) + _midPoint;
  //p2 = e1 * _radius + _midPoint;
  //p3 = e1 * (-_radius) + _midPoint;

  //xmin = static_cast<float>(std::min(p0.x, std::min(p1.x,
  //                          std::min(p2.x, p3.x) ) ) );
  //xmax = static_cast<float>(std::max(p0.x, std::max(p1.x, 
  //                          std::max(p2.x, p3.x) ) ) );
  //ymin = static_cast<float>(std::min(p0.y, std::min(p1.y, 
  //                          std::min(p2.y, p3.y) ) ) );
  //ymax = static_cast<float>(std::max(p0.y, std::max(p1.y, 
  //                          std::max(p2.y, p3.y) ) ) );
  //zmin = static_cast<float>(std::min(p0.z, std::min(p1.z, 
  //                          std::min(p2.z, p3.z) ) ) );
  //zmax = static_cast<float>(std::max(p0.z, std::max(p1.z, 
  //                          std::max(p2.z, p3.z) ) ) );

  //_bv = new AABB(xmin - DELTA, xmax + DELTA, ymin - DELTA, ymax + DELTA,
  //               zmin - DELTA, zmax + DELTA);


  //TODO : Hier wird eine BB benutzt, die um eine Kugel passt. Das geht
  //       bestimmt besser
  _bv = new AABB(static_cast<float>(_midPoint.x - _radius - DELTA),
                 static_cast<float>(_midPoint.x + _radius + DELTA),
                 static_cast<float>(_midPoint.y - _radius - DELTA),
                 static_cast<float>(_midPoint.y + _radius + DELTA),
                 static_cast<float>(_midPoint.z - _radius - DELTA),
                 static_cast<float>(_midPoint.z + _radius + DELTA) );
}


//_____________________________________________________________________________
Point
Disc::getSample()
{
  double x, y;
  float r, phi;
  Vector rVec;

  //Finde einen beliebigen Vektor der orthogonal zu Normalen auf der Scheibe ist.
  Vector orthVec = cross(_normal, Vector(1,0,0) );
  if (orthVec == Vector(0,0,0) )
    rVec = Vector(0, _radius, 0);
  else
    rVec = orthVec.norm() * _radius;

  _uSampler->createSample(x, y);
  uSquareToUDisc(static_cast<float>(x), static_cast<float>(y), r, phi);

  rVec = RotMat(_normal, phi) * rVec * r;

  Point result = _midPoint + rVec;

  return result;
}


//_____________________________________________________________________________
float
Disc::pdf(const ShadeRec& sr) const
{
  return _invArea;
}


//_____________________________________________________________________________
Normal
Disc::getNormal(const ShadeRec& sr) const
{
  return _normal;
}


//_____________________________________________________________________________
float
Disc::getArea() const
{
  return static_cast<float>(PI * _radius * _radius);
}


//_____________________________________________________________________________
void
Disc::uSquareToUDisc(float sqX, float sqY, float& r, float& phi)
{
  float a = 2 * sqX - 1;
  float b = 2 * sqY - 1;

  if (a == 0.f && b == 0.f)
  {
    r = 0.f;
    phi = 0.f;
  }
  else if (a*a> b*b)
  { // use squares instead of absolute values
    r = a;
    phi = static_cast<float>((PI/4)*(b/a) );
  }
  else
  {
    r = b;
    phi = static_cast<float>((PI/2) - (PI/4)*(a/b) );
  }
} 