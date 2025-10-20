#include "stdafx.h"

//#include "randoms.h"


//_____________________________________________________________________________
RandomS::RandomS(void)
  : UnitSampler()
{
}


//_____________________________________________________________________________
RandomS::RandomS(uint numSamples)
  : UnitSampler(numSamples)
{
  assert(_numSamples > 0);
}


//_____________________________________________________________________________
RandomS::~RandomS(void)
{
}


//_____________________________________________________________________________
RandomS*
RandomS::clone() const
{
  RandomS* tmp = new RandomS(*this);


  return tmp;
}


//_____________________________________________________________________________
uint
RandomS::setNumSamples(uint numSamples)
{
  assert(numSamples > 0);

  _numSamples = numSamples;

  return _numSamples;
}


//_____________________________________________________________________________
uint
RandomS::getNumSamples() const
{
  return _numSamples;
}


//_____________________________________________________________________________
uint
RandomS::createSample(double& x, double& y)
{
  x = randUI();
  y = randUI();

  return ++_counter;
}


//_____________________________________________________________________________
void
RandomS::reset()
{
  _counter = 0;
}