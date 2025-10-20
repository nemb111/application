
#ifndef IMAGEIO_H_
#define IMAGEIO_H_

#include <vector>

#include "IL\il.h"
#include "spectrum.h"

typedef std::vector<std::vector<Spectrum> > PixelDataRGB;
typedef std::vector<std::vector<RGBASpectrum> > PixelDataRGBA;

namespace IMAGEIO
{

void writeImage(const std::wstring& filename,unsigned int xres,
               unsigned int yres, const PixelDataRGB& pixelDataRGB);

void writeImage(const std::wstring& filename,unsigned int xres,
               unsigned int yres, const PixelDataRGBA& pixelDataRGBA);

void readImage(const std::wstring& filename, PixelDataRGB& pixelDataRGB);

void readImage(const std::wstring& filename, PixelDataRGBA& pixelDataRGBA);

}


#endif //IMAGEIO_H_