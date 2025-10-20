#include "stdafx.h"

//#include "midpoints.h"


//_____________________________________________________________________________
MidpointS::MidpointS(void)
  : UnitSampler()
{
}


////_____________________________________________________________________________
//MidpointS::MidpointS(uint numSamples)
//  : UnitSampler(numSamples)
//{
//}


//_____________________________________________________________________________
MidpointS::~MidpointS(void)
{
}


//_____________________________________________________________________________
MidpointS* 
MidpointS::clone() const
{
  MidpointS* tmp = new MidpointS(*this);


  return tmp;
}


//_____________________________________________________________________________
uint
MidpointS::createSample(double& x, double& y)
{
  x = 0.5;
  y = 0.5;

  return ++_counter;
}