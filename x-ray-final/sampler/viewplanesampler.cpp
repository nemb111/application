#include "stdafx.h"

//#include "viewplanesampler.h"


//_____________________________________________________________________________
ViewPlaneSampler::ViewPlaneSampler(void)
{
}


//_____________________________________________________________________________
ViewPlaneSampler::ViewPlaneSampler(UnitSampler* unitSampler)
  : _unitSampler(unitSampler)
{
}


//_____________________________________________________________________________
ViewPlaneSampler::~ViewPlaneSampler(void)
{
}


//_____________________________________________________________________________
void
ViewPlaneSampler::addUnitSampler(UnitSampler* unitSampler)
{
  _unitSampler = unitSampler;
}


//_____________________________________________________________________________
void
ViewPlaneSampler::getPxlSamples(PxlSampleList& pxlSampleList) const
{
  assert(pxlSampleList.size() == 0);

  PxlSample ps;
  uint numSamples = _unitSampler->getNumSamples();

  for (uint i=0; i<numSamples; i++)
  {
    _unitSampler->createSample(ps.xcoord, ps.ycoord);
    ps.weight = 1.f / numSamples;
    pxlSampleList.push_back(ps);
  }

  _unitSampler->reset();

}