#include "stdafx.h"

//#include "light.h"


//_____________________________________________________________________________
Light::Light(void)
  : _castsShadow(true)
{
  _samplePoint.push_back(Point(0,0,0) );
}


//_____________________________________________________________________________
Light::Light(bool castsShadow)
  : _castsShadow(castsShadow)
{
  _samplePoint.push_back(Point(0,0,0) );
}


//_____________________________________________________________________________
Light::~Light(void)
{
}


//_____________________________________________________________________________
const std::vector<const Point>&
Light::getSamplePoints(const ShadeRec& sr) const
{
  return _samplePoint;
}


//_____________________________________________________________________________
bool
Light::castsShadow() const
{
  return _castsShadow;
}
