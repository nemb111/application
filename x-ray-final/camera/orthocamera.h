#pragma once

#ifndef ORTHOCAMERA_H_
#define ORTHOCAMERA_H_

#include <vector>

#include "camera.h"

class OrthoCamera : public Camera
{
public:
  static void getPxlMidPnt(uint pxlIdxX, uint pxlIdxY, uint xres, uint yres, 
                           double pixelSize, double& xcoord, double& ycoord);

  OrthoCamera();
  //OrthoCamera(const Film* film, ViewPlaneSampler* viewPlaneSampler);
  virtual ~OrthoCamera(void);

  virtual bool getNextNSamples(uint nSamples, SampleList& sampleList);
  virtual int getSampleLists(const int& ntasks, 
                             std::vector<SampleList>& sampleLists);
  /*virtual bool getRayOrigins(int pixelX, int pixelY, std::vector<Point>* points) const;*/

};

#endif//ORTHOCAMERA_H_