// Bessere Implementation des Boxfilters. Laufzeit nur noch O(m^2 * 2n),
// wobei m Bildbreite und n Bildhöhe in Pixel.
// Quelle: wjarosz_convolution_2001.pdf

#pragma once

#ifndef BOXFILTER2_H_
#define BOXFILTER2_H_

class BoxFilter2 : public Filter
{
public:
  BoxFilter2(uint xWidth, uint yWidth);
  ~BoxFilter2(void);

  virtual void init(const Spectrum2D& spec2D);
  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const;


protected:
  Spectrum2D _hSpec2D; // Hier wird die horizontale Faltung durchgeführt
  float _xInvWeight;
  float _yInvWeight;


};

#endif // BOXFILTER2_H_