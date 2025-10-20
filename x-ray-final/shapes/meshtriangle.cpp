#include "stdafx.h"


//#include <algorithm>
//#include <limits>
//
//#include "aabb.h"
//#include "constants.h"
//#include "obj.h"
//#include "meshtriangle.h"
//#include "triangle.h"



////_____________________________________________________________________________
//bool
//MTTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
//{
//  Vector e1 = _p1 - _p0;
//  Vector e2 = _p2 - _p0;
//  Vector s1  = cross(ray.d(), e2);
//  double divisor = s1 * e1;
//
//  if (divisor == 0.)
//    return false;
//  double invDivisor = 1.f / divisor;
//
//  Vector d = ray.o() - _p0;
//  double b1 = d * s1 * invDivisor;
//  if (b1 < 0. || b1 > 1.)
//    return false;
//
//  Vector s2 = cross(d, e1);
//  double b2 = ray.d() * s2 * invDivisor;
//  if (b2 < 0. || b1 + b2 > 1.)
//    return false;
//
//  double t = e2 * s2 * invDivisor;
//  if (t <= KEPSILON)
//    return false;
//
//  tmin = t;
//  sr.localHitPoint = ray.o() + tmin * ray.d();
//  sr.hitPoint = sr.localHitPoint;
//  sr.hitAnObject = true;
//  sr.material = _material;
//  sr.normal = _normal;
//  sr.t = tmin;
//  sr.ray = ray;
//
//  _beta = b1;
//  _gamma = b2;
//
//  return true;
//}
//
//
////_____________________________________________________________________________
//bool
//MTTriangle::shadowHit(const Ray& ray, double& tmin) const
//{
//  Vector e1 = _p1 - _p0;
//  Vector e2 = _p2 - _p0;
//  Vector s1  = cross(ray.d(), e2);
//  double divisor = s1 * e1;
//
//  if (divisor == 0.)
//    return false;
//  double invDivisor = 1.f / divisor;
//
//  Vector d = ray.o() - _p0;
//  double b1 = d * s1 * invDivisor;
//  if (b1 < 0. || b1 > 1.)
//    return false;
//
//  Vector s2 = cross(d, e1);
//  double b2 = ray.d() * s2 * invDivisor;
//  if (b2 < 0. || b1 + b2 > 1.)
//    return false;
//
//  double t = e2 * s2 * invDivisor;
//  if (t < KEPSILON)
//    return false;
//
//  tmin = t;
//  _beta = b1;
//  _gamma = b2;
//  return true;
//}


//_____________________________________________________________________________
MeshTriangle::MeshTriangle(void)
  : _switchNormal(false), _filename(_T("") ), _meshParser(NULL), 
    _accelerator(NULL)
{
}


//_____________________________________________________________________________
MeshTriangle::MeshTriangle(const stdstring& filename,
                           const Accelerator* accelerator/*=NULL*/)
  : _switchNormal(false), _filename(filename), _meshParser(NULL),
    _accelerator(NULL)
{
  if (!selectMeshParser(filename) )
  {
    EOUT("Es konnte kein geeigneter Parser für die Datei: \"" << filename <<
         "\" gefunden werden.");
    std::cin.get();
    exit(1);
  }
  if (!_meshParser->fill(_mesh) )
  {
    EOUT("Etwas ist beim Parsen der Datei schief gelaufen.");
    std::cin.get();
    exit(1);
  }

  
  createBV();
  createTriangles();
  
  if (accelerator)
  {
    IOUT("Erzeuge Beschleunigungsdatenstruktur für: \"" << filename << "\".");
    
    setAccelerator(accelerator);
    
  }
}


//_____________________________________________________________________________
MeshTriangle::~MeshTriangle(void)
{
  delete _meshParser;
  _meshParser = 0;
  delete _accelerator;
  _accelerator = 0;
  for (size_t i=0; i<_triangles.size(); i++)
  {
    delete _triangles[i];
    _triangles[i] = 0;
  }
}


//_____________________________________________________________________________
const BV&
MeshTriangle::getBV() const
{
  assert(_bv);
  return *_bv;
}


//_____________________________________________________________________________
Normal
MeshTriangle::computeNormal() const
{
  WOUT("Die Funktion ist nicht implementiert und sollte von der ableitenden"
        "Klasse implementiert werden.");
  return Normal();
}


//_____________________________________________________________________________
void
MeshTriangle::switchNormal(bool b)
{
  _switchNormal = b;
}


//_____________________________________________________________________________
bool
MeshTriangle::selectMeshParser(const stdstring& filename)
{
  if(filename.substr(filename.find_last_of(_T("." )) + 1) == _T("obj") )
  {
    _meshParser = new OBJ(filename);
    return true;
  }
  else
    return false;
}


//_____________________________________________________________________________
void
MeshTriangle::createBV()
{
  delete _bv;
  _bv = 0;

  float xMin, yMin, zMin, xMax, yMax, zMax;
  xMin = yMin = zMin = std::numeric_limits<float>::max();
  xMax = yMax = zMax = -std::numeric_limits<float>::max();

  for (size_t i=0; i<_mesh.vertices.size(); i++)
  {
    xMin = std::min(static_cast<float>(_mesh.vertices[i][0]), xMin);
    yMin = std::min(static_cast<float>(_mesh.vertices[i][1]), yMin);
    zMin = std::min(static_cast<float>(_mesh.vertices[i][2]), zMin);
    xMax = std::max(static_cast<float>(_mesh.vertices[i][0]), xMax);
    yMax = std::max(static_cast<float>(_mesh.vertices[i][1]), yMax);
    zMax = std::max(static_cast<float>(_mesh.vertices[i][2]), zMax);
  }

  _bv = new AABB(xMin, xMax, yMin, yMax, zMin, zMax);
}


//_____________________________________________________________________________
void
MeshTriangle::createTriangles()
{
  size_t nFaces = _mesh.faces.size();
  Point p0, p1, p2;

  _triangles.reserve(nFaces);
 
  for (size_t i=0; i<nFaces; i++)
  {
    for (size_t j=0; j<3; j++)
    {
      p0[j] = _mesh.vertices[_mesh.faces[i][0] ] [j];
      p1[j] = _mesh.vertices[_mesh.faces[i][1] ] [j];
      p2[j] = _mesh.vertices[_mesh.faces[i][2] ] [j];
    }
    Triangle* tri = new Triangle(p0, p1, p2);
    _triangles.push_back(tri);

  }
}


//_____________________________________________________________________________
bool
MeshTriangle::setAccelerator(const Accelerator* accelerator)
{
  assert(accelerator);
  if (_accelerator)
    delete _accelerator;
  _accelerator = accelerator->newInstance();
  _accelerator->addShapes(_triangles);
  IOUT("Erzeuge Mesh aus Datei \""<< _filename << "\".");
  _accelerator->create();

  return true;
}


