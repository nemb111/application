#include "stdafx.h"


//#include <algorithm>
//
//#include "aabb.h"
//#include "assimpmeshtriangle.h"
//#include "triangle.h"


//_____________________________________________________________________________
AssimpMeshTriangle::AssimpMeshTriangle(const Mesh& mesh)
  : _switchNormal(false), _accelerator(NULL), _mesh(mesh)
{
  createBV();
  createTriangles();
}


//_____________________________________________________________________________
AssimpMeshTriangle::~AssimpMeshTriangle(void)
{
  delete _bv;
  _bv = 0;

  for (std::vector<Shape*>::iterator it = _triangles.begin();
       it != _triangles.end(); it++)
    delete *it;
}


//_____________________________________________________________________________
bool
AssimpMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  assert(_accelerator);


  bool hit;
  size_t shpIdx;

  hit = _accelerator->hit(ray, shpIdx, &tmin);

  if (hit)
  {
    // Berechne beta und gamma im Baryzentrischen Koordinatensystem
    Point p0 = dynamic_cast<Triangle*>(_triangles[shpIdx])->getP0();
    Point p1 = dynamic_cast<Triangle*>(_triangles[shpIdx])->getP1();
    Point p2 = dynamic_cast<Triangle*>(_triangles[shpIdx])->getP2();
    Vector e1 = p1 - p0;
    Vector e2 = p2 - p0;
    Vector s1  = cross(ray.d(), e2);
    double divisor = s1 * e1;
    double invDivisor = 1.f / divisor;
    Vector d = ray.o() - p0;
    double beta = d * s1 * invDivisor;
    Vector s2 = cross(d, e1);
    double gamma = ray.d() * s2 * invDivisor;


    sr.hitPoint = ray.o() + tmin * ray.d();
    sr.localHitPoint = ray.o() + tmin * ray.d();
    sr.hitAnObject = true;
    sr.material = _material;
    sr.uv.u = computeU(shpIdx, beta, gamma);
    sr.uv.v = computeV(shpIdx, beta, gamma);
    sr.normal = computeNormal(shpIdx, beta, gamma);
    sr.t = tmin;
    //sr.ray = ray;

    if (_switchNormal)
      sr.normal.invIP();
  }

  return hit;

  //  // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
  //  // lediglich zu Debugzwecken
  //  bool hit = false;
  //  size_t shpIdx = 0;
  //  ShadeRec result(sr.world);
  //  double t = std::numeric_limits<double>::max();
  //  size_t nTriangles = _triangles.size();
  //
  //  for (size_t i=0; i<nTriangles; i++)
  //  {
  //    if (_triangles[i]->hit(ray, tmin, sr) && tmin < t)
  //    {
  //      shpIdx = i;
  //      result = sr;
  //      t = tmin;
  //      hit = true;
  //    }
  //  }

  //  tmin = t;
  //  sr = result;
  //  sr.material = _material;
  //  sr.normal = computeNormal(shpIdx);

  //  if (_switchNormal)
  //    sr.normal.invIP();

  //return hit;

}


//_____________________________________________________________________________
bool
AssimpMeshTriangle::shadowHit(const Ray& ray, double& tmin) const
{
  assert(_accelerator);
   
  return _accelerator->shadowHit(ray, tmin);

    // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
    // lediglich zu Debugzwecken
   /* bool hit = false;
    double t = std::numeric_limits<double>::max();
    size_t nTriangles = _triangles.size();

    for (size_t i=0; i<nTriangles; i++)
    {
      if (_triangles[i]->shadowHit(ray, tmin) && tmin < t)
      {
        t = tmin;
        hit = true;
      }
    }
    tmin = t;
  
    return hit;*/
}


//_____________________________________________________________________________
bool
AssimpMeshTriangle::setAccelerator(const Accelerator* accelerator)
{
  assert(accelerator);
  
  _accelerator = accelerator->newInstance();
  _accelerator->addShapes(_triangles);
  _accelerator->create();

  return true;
}


//_____________________________________________________________________________
void
AssimpMeshTriangle::switchNormal(bool b)
{
  _switchNormal = b;
}


//_____________________________________________________________________________
Normal
AssimpMeshTriangle::computeNormal(size_t shpIdx, double beta, double gamma) 
                    const
{
  Triangle* tri = dynamic_cast<Triangle*>(_triangles[shpIdx]);

  if (tri == NULL)
  {
    EOUT("Das gecastete Objekt sollte den Laufzeittyp \"Triangle\" "
          "besitzen. Dies ist scheinbar nicht der Fall");
    std::cin.get();
    exit(1);
  }
      
  Vector normal1 = _mesh.normals[_mesh.faces[shpIdx][0] ];
  Vector normal2 = _mesh.normals[_mesh.faces[shpIdx][1] ];
  Vector normal3 = _mesh.normals[_mesh.faces[shpIdx][2] ];

  return ((1. - beta - gamma ) * normal1 + beta * normal2 + gamma * normal3 );

}


//_____________________________________________________________________________
float
AssimpMeshTriangle::computeU(size_t shpIdx, double beta, double gamma) const
{
  if (_mesh.u.size() == 0 )
    return 0.f;

  Triangle* tri = dynamic_cast<Triangle*>(_triangles[shpIdx]);

  if (tri == NULL)
  {
    EOUT("Das gecastete Objekt sollte den Laufzeittyp \"Triangle\" "
          "besitzen. Dies ist scheinbar nicht der Fall");
    std::cin.get();
    exit(1);
  }

  float u1 = _mesh.u[_mesh.faces[shpIdx][0] ];
  float u2 = _mesh.u[_mesh.faces[shpIdx][1] ];
  float u3 = _mesh.u[_mesh.faces[shpIdx][2] ];

  return static_cast<float>(
    (1. - beta - gamma ) * u1 + beta * u2 + gamma * u3 );

}


//_____________________________________________________________________________
float
AssimpMeshTriangle::computeV(size_t shpIdx, double beta, double gamma) const
{
  if (_mesh.v.size() == 0 )
    return 0.f;

  Triangle* tri = dynamic_cast<Triangle*>(_triangles[shpIdx]);

  if (tri == NULL)
  {
    EOUT("Das gecastete Objekt sollte den Laufzeittyp \"MTTriangle\" besitzen."
         " Dies ist scheinbar nicht der Fall");
    std::cin.get();
    exit(1);
  }

  float v1 = _mesh.v[_mesh.faces[shpIdx][0] ];
  float v2 = _mesh.v[_mesh.faces[shpIdx][1] ];
  float v3 = _mesh.v[_mesh.faces[shpIdx][2] ];

  return static_cast<float>(
    (1. - beta - gamma ) * v1 + beta * v2 + gamma * v3 );
}


//_____________________________________________________________________________
void
AssimpMeshTriangle::createBV()
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
AssimpMeshTriangle::createTriangles()
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
