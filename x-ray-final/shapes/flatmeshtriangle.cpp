#include "stdafx.h"

//#include <limits>
//
//#include "flatmeshtriangle.h"
//#include "parser.h"
//#include "triangle.h"

//_____________________________________________________________________________
//FlatMeshTriangle::FlatMeshTriangle(void)
//{
//}


//_____________________________________________________________________________
FlatMeshTriangle::FlatMeshTriangle(const stdstring& filename,
                                   const Accelerator* accelerator/*=NULL*/)
 : MeshTriangle(filename, accelerator)
{
}


//_____________________________________________________________________________
FlatMeshTriangle::~FlatMeshTriangle(void)
{
}


//_____________________________________________________________________________
bool
FlatMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  assert(_accelerator);
  
  bool hit;
  hit = _accelerator->hit(ray, tmin, sr);

  if (_switchNormal)
    sr.normal.invIP();

  return hit;
  
   // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
   // lediglich zu Debugzwecken
   /* bool hit = false;
    ShadeRec result(sr.world);
    double t = std::numeric_limits<double>::max();
    size_t nTriangles = _triangles.size();
  
    for (size_t i=0; i<nTriangles; i++)
    {
      if (_triangles[i]->hit(ray, tmin, sr) && tmin < t)
      {
        result = sr;
        t = tmin;
        hit = true;
      }
    }

    tmin = t;
    sr = result;
    if (_switchNormal)
      sr.normal.invIP();

  return hit;*/


}


//_____________________________________________________________________________
bool
FlatMeshTriangle:: shadowHit(const Ray& ray, double& tmin) const
{
  assert(_accelerator);

  return _accelerator->shadowHit(ray, tmin);
  
  // Dieser Abschnitt führt den Schnitttest ohne Accelerator durch und dient
  // lediglich zu Debugzwecken
 /*   bool hit = false;
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
void
FlatMeshTriangle::setMaterial(Material* material)
{
  for (size_t i=0; i<_triangles.size(); i++)
  {
    _triangles[i]->setMaterial(material);
  }
}


//_____________________________________________________________________________
MeshTriangle*
FlatMeshTriangle::clone() const
{
  FlatMeshTriangle* tmp = new FlatMeshTriangle(*this);
  tmp->_meshParser = _meshParser->clone();
  tmp->_accelerator = _accelerator->clone();
  tmp->_material = new Material(*_material);
  GBin.mrkDel<Shape*>(tmp);
  GBin.mrkDel<Material*>(const_cast<Material*>(tmp->_material) );

  return tmp;
}