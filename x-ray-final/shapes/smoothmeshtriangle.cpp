#include "stdafx.h"

//#include "parser.h"
//#include "smoothmeshtriangle.h"


//_____________________________________________________________________________
SmoothMeshTriangle::SmoothMeshTriangle(const stdstring& filename,
                                       const Accelerator* accelerator/*=NULL*/)
 : MeshTriangle(filename, accelerator)
{
}


//_____________________________________________________________________________
SmoothMeshTriangle::~SmoothMeshTriangle(void)
{
}


//_____________________________________________________________________________
bool
SmoothMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
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
    double invDivisor = 1. / divisor;
    Vector d = ray.o() - p0;
    double beta = d * s1 * invDivisor;
    Vector s2 = cross(d, e1);
    double gamma = ray.d() * s2 * invDivisor;


    sr.localHitPoint = ray.o() + tmin * ray.d();
    sr.hitPoint = sr.localHitPoint;
    sr.hitAnObject = true;
    sr.material = _material;
    sr.normal = computeNormal(shpIdx, beta, gamma);
    sr.t = tmin;
    sr.ray = ray;

    if (_switchNormal)
      sr.normal.invIP();
  }

  return hit;

    // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
    // lediglich zu Debugzwecken
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
SmoothMeshTriangle:: shadowHit(const Ray& ray, double& tmin) const
{
  assert(_accelerator);
  
  return _accelerator->shadowHit(ray, tmin);
  
    // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
    // lediglich zu Debugzwecken
    //bool hit = false;
    //double t = std::numeric_limits<double>::max();
    //size_t nTriangles = _triangles.size();

    //for (size_t i=0; i<nTriangles; i++)
    //{
    //  if (_triangles[i]->shadowHit(ray, tmin) && tmin < t)
    //  {
    //    t = tmin;
    //    hit = true;
    //  }
    //}
    //tmin = t;
  
    //return hit;
}


//_____________________________________________________________________________
MeshTriangle*
SmoothMeshTriangle::clone() const
{
  SmoothMeshTriangle* tmp = new SmoothMeshTriangle(*this);
  tmp->_meshParser = _meshParser->clone();
  tmp->_accelerator = _accelerator->clone();
  tmp->_material = new Material(*_material);
  GBin.mrkDel<Shape*>(tmp);
  GBin.mrkDel<Material*>(const_cast<Material*>(tmp->_material) );

  return tmp;
}


//_____________________________________________________________________________
Normal
SmoothMeshTriangle::computeNormal(size_t shpIdx, double beta, double gamma)
                    const
{
  switch (_mesh.normalType)
  {
    case CALCNORMAL:
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

      return ((1. - beta - gamma ) * normal1 + beta * normal2 + 
              gamma * normal3 );
    }
    case FILENORMAL:
    {
      Triangle* tri = dynamic_cast<Triangle*>(_triangles[shpIdx]);

      if (tri == NULL)
      {
        EOUT("Das gecastete Objekt sollte den Laufzeittyp \"Triangle\" "
             "besitzen. Dies ist scheinbar nicht der Fall");
        std::cin.get();
        exit(1);
      }

      size_t point1Idx = _mesh.faces[shpIdx][0];
      size_t point2Idx = _mesh.faces[shpIdx][1];
      size_t point3Idx = _mesh.faces[shpIdx][2];
      Vector normal1 = _mesh.normals[_mesh.fileNormalsIdx[shpIdx].at(point1Idx)];
      Vector normal2 = _mesh.normals[_mesh.fileNormalsIdx[shpIdx].at(point2Idx)];
      Vector normal3 = _mesh.normals[_mesh.fileNormalsIdx[shpIdx].at(point3Idx)];

      return ((1. - beta - gamma ) * normal1 + beta * normal2 + 
              gamma * normal3 );
    }
  }

  return Normal();
}


//_____________________________________________________________________________
//void
//SmoothMeshTriangle::setMaterial(Material* material)
//{
//  for (size_t i=0; i<_triangles.size(); i++)
//  {
//    _triangles[i]->setMaterial(material);
//  }
//}