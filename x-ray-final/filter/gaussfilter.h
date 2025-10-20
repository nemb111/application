#pragma once

#ifndef GAUSSFILTER
#define GAUSSFILTER


#include "filter.h"
class GaussFilter : public Filter
{
public:
  GaussFilter(float sigma=1.f);
  ~GaussFilter(void);


  virtual void init(const Spectrum2D& spec2D);
  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

protected:
  Spectrum2D _hSpec2D; // Hier wird die horizontale Faltung durchgeführt
  std::vector<double> _1DKernel;

};


#endif // GAUSSFILTER