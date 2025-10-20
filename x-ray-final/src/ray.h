/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 50ff, 53ff.
*/

#pragma once

#ifndef RAY_H_
#define RAY_H_

#include "linAlg.h"
#include "material.h"
#include "texture.h"
#include "world.h"

//Forward declarations
class Material;
class World;
struct UV;

class Ray
{
public:
  Ray(void);
  Ray(const Point& origin, const Vector& direction);
  virtual ~Ray(void);

  virtual const Point& o() const {return _o;} 
  virtual const Vector& d() const {return _d;}
  virtual bool operator==(const Ray& rhs) const;

protected:
  Point  _o;
  Vector _d;
};


class InvRay : public Ray
{
public:
  InvRay(const Point& origin, const Vector& direction);
  virtual ~InvRay(void){}

};



class ShadeRec {
public:
	
	bool			      	hitAnObject;		// Did the ray hit an object?
	const Material*  	material;		// Pointer to the nearest object's material
	Point          		hitPoint;			// World coordinates of intersection
	Point       			localHitPoint;	// World coordinates of hit point on generic object (used for texture transformations)
  Point             lightSamplePnt; // World coordinates of sample point on light surface
	Normal      			normal;				// Normal at hit point
  UV                uv;             // uv texture coordinates
	Ray			      		ray;				// Required for specular highlights and area lights
	int     					depth;				// recursion depth
	double		      	t;					// ray parameter
  HMatrix           transMat;        // Object transformation matrix
  HMatrix           invTransMat;    // Inverse object transformation matrix
  float             contribution; // The rays contribution to the final pixel color
	const World&		  world;					// World reference

  
	//Spectrum          spectrum;
				
  ShadeRec(const World& wr);					// constructor
		
	ShadeRec(const ShadeRec& sr);			// copy constructor

  ShadeRec& operator=(const ShadeRec& rhs);
};


#endif//RAY_H_