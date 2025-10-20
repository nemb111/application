#pragma once

#ifndef RANDOMS_H_
#define RANDOMS_H_

#include "unitsampler.h"
class RandomS : public UnitSampler
{
public:
  RandomS(void);
  RandomS(uint numSamples);
  virtual ~RandomS(void);

  virtual RandomS* clone() const;

  virtual uint setNumSamples(uint numSamples);
  virtual uint getNumSamples() const;
  // Die Methode gibt die Zahl bereits erzeugter Samples zurück. 
  virtual uint createSample(double& x, double& y);
  virtual std::string className() const {return "RandomS";}
  virtual void reset();
};

#endif //RANDOMS_H_