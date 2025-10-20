/*
  Das Interface dieser Klasse ist angelehnt am Interface der Filterklasse aus 
  dem Buch "Physically based Rendering" S. 393
*/

#pragma once

#ifndef FILTER_H_
#define FILTER_H_

#include <vector>

#include "spectrum.h"

typedef std::vector<std::vector<Spectrum> > Spectrum2D;

class Filter
{
public:
  Filter(uint xWidth, uint yWidth);
  virtual ~Filter(void);

  virtual void init(const Spectrum2D& spec2D);
  virtual Spectrum evaluate(const Spectrum2D& spec2D, uint x, uint y) const = 0;

protected:

  size_t calcI(size_t index, int offset, size_t picSize) const;
  size_t calcPI(uint index, uint width, size_t size) const;
  size_t calcNI(uint index, uint width) const;


  uint _xWidth;
  uint _yWidth;

};

#endif //FILTER_H_