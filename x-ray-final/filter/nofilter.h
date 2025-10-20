#pragma once

#ifndef NOFILTER_H_
#define NOFILTER_H_

#include "filter.h"

class NoFilter : public Filter
{
public:
  NoFilter(void);
  virtual ~NoFilter(void);

  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

};

#endif //NOFILTER_H_