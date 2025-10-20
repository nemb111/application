/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 479ff.
*/

#pragma once

#ifndef FLATMESHTRIANGLE_H_
#define FLATMESHTRIANGLE_H_

#include "meshtriangle.h"
class FlatMeshTriangle : public MeshTriangle
{
public:
  //FlatMeshTriangle(void);
  FlatMeshTriangle(const stdstring& filename,
                   const Accelerator* accelerator=NULL);
  virtual ~FlatMeshTriangle(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual void setMaterial(Material* material);

  virtual MeshTriangle* clone() const;

protected:
  
};

#endif//FLATMESHTRIANGLE_H_