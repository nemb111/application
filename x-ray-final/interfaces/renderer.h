/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 263ff.
*/


#pragma once

#ifndef RENDERER_H_
#define RENDERER_H_

#include "camera.h"
#include "ray.h"
#include "spectrum.h"
#include "world.h"

//Forward Declarations
struct Sample;
class World;
class ShadeRec;
class Ray;

typedef std::vector<Sample> SampleList;
//typedef std::vector<std::pair<float, Sample> > PrioSampleList;

class Renderer {
public:
  // Renderer Interface
  Renderer(float cullCoeff=0.001f) 
    : _maxDepth(1), /*_maxGlobIlDepth(1),*/ _cullCoeff(cullCoeff),
      _debugPixelSet(false), _debugPixelX(0), _debugPixelY(0) {}
  virtual ~Renderer(){}
  virtual void init(const World* world);
  // true zurückgeben zum Weitermachen; false zum Stoppen.
  virtual bool Render(const SampleList& sampleList, uint iteration) const;
  //virtual void GlobIllRender(PrioSampleList& prioSampleList);
  // depth gibt die Rekursionstiefe an für die reflektierte und gebrochene
  // rays berechnet werden.
  // contribution gibt den prozentualen Anteil eines Ray bei der Berechnung
  // des terminalen Farbwertes an.
  virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f) const = 0;
  //virtual Spectrum globalLi(const Ray& ray, int depth,
  //                          float contribution=1.f) const = 0;
  virtual void writeBackSample(uint pxlIdxX, uint pxlIdxY,
                               const Spectrum& spectrum) const;
  virtual void writeBackGlobIllSample(uint pxlIdxX, uint pxlIdxY,
                                      const Spectrum& spectrum) const;
  virtual void setDebugPixel(unsigned int x, unsigned int y);


protected:
  int    _maxDepth;
  //int    _maxGlobIlDepth;
  float  _cullCoeff;
  const World* _world;

  bool _debugPixelSet;
  uint _debugPixelX;
  uint _debugPixelY;
};



#endif //RENDERER_H_
