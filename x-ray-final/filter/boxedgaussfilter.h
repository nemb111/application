#pragma once

#ifndef BOXEDGAUSSFILTER_H_
#define BOXEDGAUSSFILTER_H_


#include "boxfilter3.h"
class BoxedGaussFilter : public BoxFilter3
{
public:
  BoxedGaussFilter(float sigma, uint iteration=6);
  ~BoxedGaussFilter(void);

  virtual void init(const Spectrum2D& spec2D);
  //virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

protected:
  float _sigma;
  uint _iteration;
};


#endif //BOXEDGAUSSFILTER_H_