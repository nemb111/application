#pragma once

#ifndef HAMMERSLEYS_H_
#define HAMMERSLEYS_H_

#include <vector>
#include <utility>

#include "unitsampler.h"


class HammersleyS : public UnitSampler
{
public:
  HammersleyS(void);
  HammersleyS(uint numSamples, uint base=2);
  virtual ~HammersleyS(void);

  virtual HammersleyS* clone() const;

  virtual uint setNumSamples(uint numSamples);
  virtual uint getNumSamples() const;
  // Die Methode gibt die Zahl bereits erzeugter Samples zurück. 
  virtual uint createSample(double& x, double& y);
  virtual std::string className() const {return "HammersleyS";}
  virtual void reset();

protected:

  double phi(uint k) const;

  uint _base;
  std::vector<std::pair<double, double> > _samples;
};


#endif //HAMMERSLEYS_H_