
#pragma once

#ifndef PRENDFILTER_H_
#define PRENDFILTER_H_

#include "x-ray.h"

namespace PRF
{

enum Modus
{
  NO_FILTER,
  CONE_FILTER,
  GAUS_FILTER
};


class Params
{
private:
  float _k;  // Wird verwendet mit dem Cone-Filter. Bedingung: k >= 1.
  float _alpha; // Wird beim Gauß-Filter verwendet guter Standardwert: 0.918 siehe [Pavicic90]
  float _beta;  // Wird beim Gauß-Filter verwendet guter Standardwert: 1.953 siehe [Pavicic90]

public:

  Params() : _k(1.f), _alpha(0.918f), _beta(1.953f) {}
  Params(float k, float alpha, float beta) : _k(k), _alpha(alpha), _beta(beta) {}


  float k() const
  {
    assert(_k >= 1.f);
    return _k;
  }

  float alpha() const {return _alpha;}
  float beta() const {return _beta;}
  

};


inline float weight(Modus modus, float maxDistSqu, float currDistSqu, 
                    const Params& params)
{

  switch (modus)
  {
    case NO_FILTER:
    {
      return 1.f;
    }
    case CONE_FILTER:
    {
      return ((1.f - std::sqrtf(currDistSqu) / (params.k() * std::sqrtf(maxDistSqu) ) ) ) /
             ( 1.f - 0.66666666f * params.k() );
    }
    case GAUS_FILTER:
    {
      float numer = 1.f - std::exp(-params.beta() * currDistSqu / (2*maxDistSqu)  );
      float denom = 1.f - std::exp(-params.beta() );


      return params.alpha() * 
               (
                 1.f - (1.f - numer / denom)
               );
    }
  }


  return 1.f;
}




}

#endif //PRENDFILTER_H_