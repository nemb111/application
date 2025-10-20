#pragma once

#ifndef PPM_H_
#define PPM_H_

#include <vector>

#include "spectrum.h"

typedef std::vector<std::vector<Spectrum> > Spectrum2D;

namespace PPM
{

void writeFile(unsigned int xres, unsigned int yres, unsigned int colordepth,
               const Spectrum2D& spectrumArray);

void writeFile(const std::wstring& filename,unsigned int xres,
               unsigned int yres, unsigned int colordepth,
               const Spectrum2D& spectrumArray);

void readFile(unsigned int& xres, unsigned int& yres, unsigned int& colordepth,
              Spectrum2D& spectrumArray);

}


#endif//PPM_H_