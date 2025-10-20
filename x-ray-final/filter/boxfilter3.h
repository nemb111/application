// Bessere Implementation des Boxfilters. Laufzeit nur noch O(m * n),
// wobei m Bildbreite und n Bildhöhe in Pixel.
// Quelle: wjarosz_convolution_2001.pdf


#pragma once

#ifndef BOXFILTER3_H_
#define BOXFILTER3_H_


#include "filter.h"
class BoxFilter3 : public Filter
{
public:
  BoxFilter3(uint xWidth, uint yWidth);
  ~BoxFilter3(void);


  virtual void init(const Spectrum2D& spec2D);
  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;

  Spectrum2D _totSpec2D; // Hier werden die Vorberechneten Werte abgespeichert.
  float _xInvWeight;
  float _yInvWeight;

};

#endif // BOXFILTER3_H_