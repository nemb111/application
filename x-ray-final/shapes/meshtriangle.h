/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 478ff.
*/


#pragma once

#ifndef MESHTRIANGLE_H_
#define MESHTRIANGLE_H_

#include <string>
#include <vector>

#include "accelerator.h"
#include "linAlg.h"
#include "mesh.h"
#include "meshparser.h"
#include "shape.h"
#include "triangle.h"
#include "x-ray.h"


//Foward Deklarationen
class MeshParser;
class Normal;
class Point;


using std::vector;
using std::string;


// Diese von Triangle abgeleitete Klasse speichert nach einem erfolgreichen
// hit oder shadowHit die baryzentrischen Parameter "beta" und "gamma", damit 
// diese beim Berechnen der Normalen nicht erneut berechnet werden müssen
//class MTTriangle : public Triangle
//{
//public:
//  MTTriangle(void) : Triangle(), _beta(1./3), _gamma(1./3){}
//  MTTriangle(Point p0, Point p1, Point p2)
//    : Triangle(p0, p1, p2), _beta(1./3), _gamma(1./3){}
//
//  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
//  virtual bool shadowHit(const Ray& ray, double& tmin) const;
//
//  double getBeta(){return _beta;}
//  double getGamma(){return _gamma;}
//
//protected:
//  mutable double _beta;
//  mutable double _gamma;
//
//};


class MeshTriangle : public Shape
{
public:
  MeshTriangle(const stdstring& filename, const Accelerator* accelerator=NULL);
  virtual ~MeshTriangle(void);

  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
  virtual bool shadowHit(const Ray& ray, double& tmin) const = 0;
  // Die Referenz auf einen Accelerator wird dazu benutzt mittles 
  // "newInstance()" ein neues Objekt zu erzeugen. Damit benutzt ein Mesh
  // immer den gleichen Accelerator wie World.
  virtual bool setAccelerator(const Accelerator* accelerator);
  virtual const BV& getBV() const;
  
  virtual Normal computeNormal() const;
  virtual void switchNormal(bool b);
  

protected:
  MeshTriangle(void);

  virtual void createBV();
  virtual void createTriangles();
  virtual bool selectMeshParser(const stdstring& filename);


  virtual MeshTriangle* clone() const = 0;

  bool _switchNormal;
  const stdstring _filename;
  Mesh _mesh;
  MeshParser* _meshParser;
  Accelerator* _accelerator;
  std::vector<Shape*> _triangles;

};

#endif//MESHTRIANGLE_H_