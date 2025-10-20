/* Das Interface in dieser Datei oder der zugehörigen cpp-Datei orientiert sich an:
   Dem Buch "Ray Tracing from the ground up" / Seite 63ff.
*/

#pragma once

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

//interfaces
#include "accelerator.h"
#include "camera.h"
#include "film.h"
#include "filter.h"
#include "guicomm.h"
#include "light.h"
#include "renderer.h"
#include "shape.h"
#include "spectrum.h"
#include "unitsampler.h"

//Forward Declarations
class Accelerator;
class Camera;
class Light;
class Renderer;
class Sampler;
class Shape;


class World
{
public:
  
  World(void);
  virtual ~World(void);


	Film*					              film;
  Camera*					           	camera;
  UnitSampler*                cameraDirIllSampler;
  UnitSampler*                cameraGlobIllSampler;
  Accelerator*                accelerator;
	Spectrum*            				backgroundColor;
	Renderer*			          		renderer;
  Renderer*                   globalRenderer;
	Light*   					          ambientLight;
  Filter*                     filter;
	std::vector<Shape*>	        shapes;		
	std::vector<Light*> 				lights;

  GUIcomm*                    guiComm;
		
	
  void
  addCamera(Camera* camera);

	void 
	addShape(Shape* shape);
		
	void 
	addLight(Light* l); 
		
	void
	setAmbientLight(Light* l);			
		
	void
	setCamera(Camera* c);	 

	void 					
	build(void);

	void 												
	renderScene(void) const;
						


	/*ShadeRec
	hit_objects(const Ray& ray);*/
		
						
private:
		
	void 
	deleteObjects(void);
		
	void 
	deleteLights(void);
};


// ------------------------------------------------------------------ addCamera
inline void
World::addCamera(Camera* cam)
{
  camera = cam;
}


// ------------------------------------------------------------------ addShape

inline void 
World::addShape(Shape* shape) {  
  shapes.push_back(shape);	
}


// ------------------------------------------------------------------ addLight

inline void 
World::addLight(Light* light) {  
	lights.push_back(light);
}


// ------------------------------------------------------------------ setAmbientLight

inline void
World::setAmbientLight(Light* l) {
	ambientLight = l;
}


// ------------------------------------------------------------------ setCamera

inline void
World::setCamera(Camera* c) {
	camera = c;
}

#endif//WORLD_H_H



