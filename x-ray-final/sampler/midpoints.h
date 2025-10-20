#pragma once

#ifndef MIDPOINT_H_
#define MIDPOINT_H_


#include "unitsampler.h"
class MidpointS : public UnitSampler
{
public:
  MidpointS(void);
  //MidpointS(uint numSamples);
  virtual ~MidpointS(void);

  virtual MidpointS* clone() const;

  virtual uint createSample(double& x, double& y);
  virtual std::string className() const {return "MidpointS";}
};


#endif //MIDPOINT_H_