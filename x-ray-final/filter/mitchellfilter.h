// Codeauszüge sind entnommen aus dem pbrt Framework

#pragma once

#ifndef MITCHELLFILTER_H_
#define MITCHELLFILTER_H_

#include "filter.h"
class MitchellFilter : public Filter
{
public:
  MitchellFilter(uint xWidth, uint yWidth, float B=0.33333, float C=0.33333);
  virtual ~MitchellFilter(void);

  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

protected:

  float Mitchell1D(float x) const 
  {
    x = fabsf(x);
    if (x < 1.f)
      return ((12 - 9*_B - 6*_C) * x*x*x +
              (-18 + 12*_B + 6*_C) * x*x +
              (6 - 2*_B)) * (1.f/6.f);
    else if (x <= 2)
      return ((-_B - 6*_C) * x*x*x + (6*_B + 30*_C) * x*x +
              (-12*_B - 48*_C) * x + (8*_B + 24*_C)) * (1.f/6.f);
    else
      return 0;
  //  if(x < 1.0f)
		//  return 7*x*x*x - 12*x*x + 16/3;
		//else if(x < 2.0f)
		//  return -7/3*x*x*x + 12*x*x - 20*x + 32/3;
		//else
		//  return 0.0f;
  }

  float _B;
  float _C;
  float _invXWidth;
  float _invYWidth;
};

#endif //MITCHELLFILTER_H_