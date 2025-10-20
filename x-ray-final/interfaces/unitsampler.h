#pragma once

#ifndef UNITSAMPLER_H_
#define UNITSAMPLER_H_

#include "x-ray.h"

// Basisklasse für alle Sampler, die Stichproben vom Einheitsquadrat ziehen
class UnitSampler
{
public:
  UnitSampler(void);
  UnitSampler(uint numSamples);
  virtual ~UnitSampler(void);

  virtual UnitSampler* clone() const = 0;
  virtual uint setNumSamples(uint numSamples);
  virtual uint getNumSamples() const;
  // Die Methode gibt die Zahl bereits erzeugter Samples zurück. 
  virtual uint createSample(double& x, double& y) = 0;
  virtual std::string className() const = 0;
  virtual uint numSamplesCreated() const {return _counter;}
  virtual void reset();

protected:

  uint _numSamples;
  uint _counter;
};

#endif //UNITSAMPLER_H_