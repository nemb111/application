/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 486ff.
*/

#pragma once

#ifndef SMOOTHMESHTRIANGLE_H_
#define SMOOTHMESHTRIANGLE_H_

#include "meshtriangle.h"



class SmoothMeshTriangle : public MeshTriangle
{
public:
  SmoothMeshTriangle(const stdstring& filename, 
                     const Accelerator* accelerator=NULL);
  virtual ~SmoothMeshTriangle(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;

  virtual MeshTriangle* clone() const;

protected:
  Normal computeNormal(size_t shpIdx, double beta, double gamma) const;
};


#endif //SMOOTHMESHTRIANGLE_H_