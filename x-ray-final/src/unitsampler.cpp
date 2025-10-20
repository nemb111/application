#include "stdafx.h"

//#include "unitsampler.h"


//_____________________________________________________________________________
UnitSampler::UnitSampler(void)
  : _numSamples(1), _counter(0)
{
}


//_____________________________________________________________________________
UnitSampler::UnitSampler(uint numSamples)
  : _numSamples(numSamples), _counter(0)
{
  assert(numSamples > 0);
}


//_____________________________________________________________________________
UnitSampler::~UnitSampler(void)
{
}


//_____________________________________________________________________________
uint
UnitSampler::setNumSamples(uint numSamples)
{
  assert(numSamples > 0);

  _counter = 0;

  return _numSamples = numSamples;
}


//_____________________________________________________________________________
uint
UnitSampler::getNumSamples() const
{
  return _numSamples;
}


//_____________________________________________________________________________
void
UnitSampler::reset()
{
  if (_counter != _numSamples)
  {
    WOUT("Der UnitSampler wurde zurückgesetzt, bevor alle Samples erzeugt "
         "wurden."); 
  }

  _counter = 0;
}