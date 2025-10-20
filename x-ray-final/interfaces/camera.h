/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 158ff.
*/


#pragma once

#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include "film.h"
#include "ray.h"
#include "viewplanesampler.h"


class Ray;


struct Sample
{
  Sample() : pxlIdxX(0), pxlIdxY(0){}
  Sample(uint x, uint y) : pxlIdxX(x), pxlIdxY(y){}

  uint pxlIdxX;
  uint pxlIdxY;

  std::vector<Ray> rays;
  std::vector<float> weigths;
};

typedef std::vector<Sample> SampleList;
typedef std::vector<std::pair<float, Sample> > PrioSampleList;



class Camera
{
public:
  virtual bool getNextNSamples(uint nSamples, SampleList& dirIllSampleList,
                               SampleList& globIllSampleList) = 0;

  Camera();
  virtual ~Camera();

  /*virtual bool getRayOrigins(int pixelX, int pixelY, 
                             std::vector<Point>* points) const = 0;*/
  virtual void getPxlOffset(uint pxlIdxX, uint pxlIdxY, double& xOff,
                            double& yOff);
  virtual int getSampleLists(const int& ntasks, 
                             std::vector<SampleList>& sampleLists){return 0;}
  virtual void setFilm(const Film* film);
  //virtual void setUnitSampler(UnitSampler* unitSampler);
  virtual void setUnitSampler(UnitSampler* dIllViewPlaneUS,
                              UnitSampler* gIllViewPlaneUS=nullptr);

protected:
  uint _nextPxlIdxX;
  uint _nextPxlIdxY;
  const Film* _film;
  //ViewPlaneSampler _viewPlaneSampler;
  ViewPlaneSampler _dIllViewPlaneS;
  ViewPlaneSampler _gIllViewPlaneS;


  //Füllt sampleList mit den nächsten n unbenutzten Pixelindizes. Gibt
  //false zurück falls die gewünschte Anzahl Pixel nicht mehr vorhanden ist.
  virtual bool getNextNSamples(uint nSamples, SampleList& sampleList);


};



#endif//CAMERA_H_

