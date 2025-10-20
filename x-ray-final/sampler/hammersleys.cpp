#include "stdafx.h"

//#include "hammersleys.h"


//_____________________________________________________________________________
HammersleyS::HammersleyS(void)
  : UnitSampler()
{
  _samples.push_back(std::make_pair<double, double>(0, 0) );
}


//_____________________________________________________________________________
HammersleyS::HammersleyS(uint numSamples, uint base/*=2*/)
  : UnitSampler(numSamples), _base(base)
{
  assert(_numSamples > 0);

  for (uint i=0; i<_numSamples; i++)
    _samples.push_back(std::make_pair<double, double>
                        (i/static_cast<double>(_numSamples),
                         phi(i) ) );
}


//_____________________________________________________________________________
HammersleyS::~HammersleyS(void)
{
}


//_____________________________________________________________________________
HammersleyS* 
HammersleyS::clone() const
{
  HammersleyS* tmp = new HammersleyS(*this);


  return tmp;
}


//_____________________________________________________________________________
uint
HammersleyS::setNumSamples(uint numSamples)
{
  assert(numSamples > 0);

  _numSamples = numSamples;
  reset();

  _samples.clear();
  for (uint i=0; i<_numSamples; i++)
    _samples.push_back(std::make_pair<double, double>
                        (i/static_cast<double>(_numSamples),
                         phi(i) ) );

  return _numSamples;
}


//_____________________________________________________________________________
uint
HammersleyS::getNumSamples() const
{
  return _numSamples;
}


//_____________________________________________________________________________
uint
HammersleyS::createSample(double& x, double& y)
{
  if (_counter > _numSamples)
  {
    WOUT("Anzahl festgelegter Samples überschritten. Undefiniertes Verhalten"
         "möglich! Verwenden der \"reset-Methode\" notwendig.");
    _counter %= _numSamples;
  }

  x = _samples[_counter].first;
  y = _samples[_counter].second;

  return ++_counter;
}


//_____________________________________________________________________________
void
HammersleyS::reset()
{
  _counter = 0;
}


//_____________________________________________________________________________
double
HammersleyS::phi(uint k) const
{
  double phi=0;
  uint a = 0;
  uint based = _base;

  while (k > 0)
  {
    a = k % _base;
    phi = phi + static_cast<double>(a) / based;
    k = static_cast<uint>(k / _base);
    based *= _base;
  }

  return phi;
}