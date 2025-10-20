#pragma once

#ifndef VIEWPLANESAMPLER_H_
#define VIEWPLANESAMPLER_H_

#include <vector>

#include "unitsampler.h"


//Forward Deklarationen
struct Sample;
class Ray;
class UnitSampler;

//Koordinaten eines einzelnen Pixelsamples.
struct PxlSample
{
  double xcoord;
  double ycoord;
  float weight;
};

typedef std::vector<PxlSample> PxlSampleList;


class ViewPlaneSampler
{
public:
  ViewPlaneSampler(void);
  ViewPlaneSampler(UnitSampler* unitSampler);
  virtual ~ViewPlaneSampler(void);

  virtual void addUnitSampler(UnitSampler* unitSampler);
  virtual void getPxlSamples(PxlSampleList& pxlSampleList) const;

protected:

  UnitSampler* _unitSampler;
};


#endif //VIEWPLANESAMPLER_H_