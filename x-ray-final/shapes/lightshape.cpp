#include "stdafx.h"

//#include "lightshape.h"

//_____________________________________________________________________________
uint LightShape::id = 0;


//_____________________________________________________________________________
LightShape::LightShape(void)
  : Shape(), _id(id++), _invArea(0), _uSampler(NULL)
{
}


//_____________________________________________________________________________
LightShape::LightShape(float invArea)
  : Shape(), _id(id++), _invArea(invArea), _uSampler(NULL)
{
}


//_____________________________________________________________________________
LightShape::~LightShape(void)
{
}


//_____________________________________________________________________________
void
LightShape::setUnitSampler(UnitSampler* uSampler)
{
  _uSampler = uSampler;
}


//_____________________________________________________________________________
void
LightShape::resetSampler()
{
  _uSampler->reset();
}


//_____________________________________________________________________________
uint
LightShape::numSamples() const
{
  return _uSampler->getNumSamples();
}