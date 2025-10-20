#pragma once

#ifndef STRATIFIEDS_H_
#define STRATIFIEDS_H_

#include "unitsampler.h"
class StratifiedS : public UnitSampler
{
public:
  StratifiedS(void);
  StratifiedS(uint numSamples);
  virtual ~StratifiedS(void);

  virtual StratifiedS* clone() const;

  virtual uint setNumSamples(uint numSamples);
  virtual uint getNumSamples() const;
  // Die Methode gibt die Zahl bereits erzeugter Samples zurück. 
  virtual uint createSample(double& x, double& y);
  virtual std::string className() const {return "StratifiedS";}
  virtual void reset();

protected:

  uint    _tilesPerSide;
  double  _sideWidth;

};


#endif //STRATIFIEDS_H_