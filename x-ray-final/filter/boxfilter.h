#pragma once

// Naive Implementation des Boxfilters Laufzeit O(m^2 * n^2), wobei m 
// Bildbreite und n Bildhöhe in Pixel
// Quelle: wjarosz_convolution_2001.pdf

#ifndef BOXFILTER_H_
#define BOXFILTER_H_

#include "filter.h"
class BoxFilter : public Filter
{
public:
  BoxFilter(uint xWidth, uint yWidth);
  virtual ~BoxFilter(void);

  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

protected:

  float _invWeight;

};

#endif //BOXFILTER_H_