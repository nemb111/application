#pragma once

#ifndef ASSIMPMESHTRIANGLE_H_
#define ASSIMPMESHTRIANGLE_H_

#include "accelerator.h"
#include "mesh.h"
#include "shape.h"


class AssimpMeshTriangle : public Shape
{
public:
  AssimpMeshTriangle(const Mesh& mesh);
  virtual ~AssimpMeshTriangle(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual bool setAccelerator(const Accelerator* accelerator);
  
  virtual void switchNormal(bool b);


protected:
  Normal computeNormal(size_t shpIdx, double beta, double gamma) const;
  float  computeU(size_t shpIdx, double beta, double gamma) const;
  float  computeV(size_t shpIdx, double beta, double gamma) const;
  virtual void createBV();
  virtual void createTriangles();


  bool _switchNormal;
  Accelerator* _accelerator;
  Mesh _mesh;
  std::vector<Shape*> _triangles;

};

#endif //ASSIMPMESHTRIANGLE_H_