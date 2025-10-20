#include "stdafx.h"

//#include "stratifieds.h"


//_____________________________________________________________________________
StratifiedS::StratifiedS(void)
  : UnitSampler(), _tilesPerSide(1), _sideWidth(1.)
{
}


//_____________________________________________________________________________
StratifiedS::StratifiedS(uint numSamples)
  : UnitSampler(roundUpSqrNum(numSamples) ), 
    _tilesPerSide(static_cast<uint>(sqrt(static_cast<double>(_numSamples+1) ) ) ),
    _sideWidth(1./_tilesPerSide)
{
}


//_____________________________________________________________________________
StratifiedS::~StratifiedS(void)
{
}


//_____________________________________________________________________________
StratifiedS*
StratifiedS::clone() const
{
  StratifiedS* tmp = new StratifiedS(*this);


  return tmp;
}


//_____________________________________________________________________________
uint
StratifiedS::setNumSamples(uint numSamples)
{
  assert(numSamples > 0);
  
  reset();

  _numSamples = roundUpSqrNum(numSamples);
  _tilesPerSide = static_cast<uint>(sqrt(static_cast<double>(_numSamples+1) ) );
  _sideWidth = 1. / _tilesPerSide;

  return _numSamples;
}


//_____________________________________________________________________________
uint
StratifiedS::getNumSamples() const
{
  return _numSamples;
}


//_____________________________________________________________________________
uint
StratifiedS::createSample(double& x, double& y)
{
  x = (_counter % _tilesPerSide) * _sideWidth + (randUI() * _sideWidth);
  y = (_counter / _tilesPerSide) * _sideWidth + (randUI() * _sideWidth);

  if (_counter > _numSamples)
    WOUT("Anzahl festgelegter Samples überschritten. Undefiniertes Verhalten"
         "möglich! Verwenden der \"reset-Methode\" notwendig.");

  return ++_counter;
}


//_____________________________________________________________________________
void
StratifiedS::reset()
{
  _counter = 0;
}