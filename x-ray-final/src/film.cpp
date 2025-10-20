#include "stdafx.h"

//#include <cstdlib>
//#include <fstream>
//
//#include "imageio.h"
//#include "film.h"
//#include "options.h"
//#include "ppm.h"


//_____________________________________________________________________________
Film::Film(int xr, int yr, double pSize, float gam, float inv_gam,
       bool s_o_o_g)
       : xres(RoundUpEven(xr) ), yres(RoundUpEven(yr) ), pixelSize(pSize),
         gamma(gam), inv_gamma(inv_gam), _invHighestCoeff(1.f),
         show_out_of_gamut(s_o_o_g)
{
  assert(xres != 0 && yres != 0 && pixelSize != 0);
  /*_resPixelArraySet = std::vector<std::vector<bool> >
                        (yres, std::vector<bool>(xres, false) );*/
  _resPixelArray = Spectrum2D(yres, std::vector<Spectrum>(xres, BLACK) );
  _dirIllPixelArray = Spectrum2D(yres, std::vector<Spectrum>(xres, BLACK) );
  _globIllPixelArray = Spectrum2D(yres, std::vector<Spectrum>(xres, BLACK) );
}


//_____________________________________________________________________________
void
Film::updateResPixelArray(int x, int y)
{
  Spectrum tmpSpec = _dirIllPixelArray[y][x] + _globIllPixelArray[y][x];
  float tmpCoeff   = tmpSpec.maxCoeff();

  // Normalisiere die in _resPixelArray gespeicherten Spectrum Werte. Für
  // Debugging Zwecke wird show_out_of_gamut auf true gesetzt. Spectren die
  // eine Komponente > 1 besitzen werden auf RED gesetzt. Ansonsten werden
  // die Komponenten auf [0,1] skaliert, so dass das proportionale Verhältnis
  // der Komponenten untereinander gewahrt wird. Vorsicht Informationsverlust
  // möglich.
  if (show_out_of_gamut && (tmpCoeff > 1.f) )
  {
    _resPixelArray[y][x] = RED;
    
    return;
  }
 /* else
  {
    _resPixelArray[y][x] = tmpSpec * _invHighestCoeff;
    if (tmpCoeff > (1.f/_invHighestCoeff) )
    {
      _invHighestCoeff = 1.f / (_invHighestCoeff * tmpCoeff);

      for (uint yIdx=0; yIdx<yres; yIdx++)
        for (uint xIdx=0; xIdx<xres; xIdx++)
        {
          _resPixelArray[yIdx][xIdx] *= _invHighestCoeff;
        }
    }
  }*/
  else
  {
    if (tmpCoeff > 1.f)
    {
      tmpSpec *= 1.f/tmpCoeff;
    }
  }

  _resPixelArray[y][x] = tmpSpec;
}


//____________________________________________________________________________
void
Film::filter(Filter* filter, GUIcomm* guicomm)
{
  Spectrum2D tmpSpec2D(yres, std::vector<Spectrum>(xres, BLACK) );

  filter->init(_resPixelArray);

  for (uint y=0; y < yres; y++)
  {
    for (uint x=0; x < xres; x++)
    {
      tmpSpec2D[y][x] = filter->evaluate(_resPixelArray, x, y);
    }
  }

  for (uint y=0; y < yres; y++)
    for (uint x=0; x < xres; x++)
    {
      _resPixelArray[y][x] = tmpSpec2D[y][x];
      guicomm->updateBitmap(x, y, _resPixelArray[y][x]);
    }


}


//_____________________________________________________________________________
const Spectrum&
Film::getGlobIllPixelSpectrum(int x, int y) const
{
  return _globIllPixelArray[y][x];
}


//_____________________________________________________________________________
const Spectrum&
Film::getResPixelSpectrum(int x, int y) const
{
  return _resPixelArray[y][x];
}


//_____________________________________________________________________________
void
Film::setDirIllPixelSpectrum(int x, int y, const Spectrum& s)
{
  _dirIllPixelArray[y][x] = s;

  updateResPixelArray(x, y);
}


//_____________________________________________________________________________
void
Film::setGlobIllPixelSpectrum(int x, int y, const Spectrum& s)
{
  _globIllPixelArray[y][x] = s;

  updateResPixelArray(x, y);
}


//_____________________________________________________________________________
void
Film::writeImage()
{
  //float compMax = 0;

  //Spectrum2D resPixelArray(
  //  Spectrum2D(yres, std::vector<Spectrum>(xres, BLACK) ) );

  //for (size_t y=0, endY=resPixelArray.size(); y<endY; y++)
  //  for (size_t x=0, endX=resPixelArray[y].size(); x<endX; x++)
  //    resPixelArray[y][x] = _pixelArray[y][x] + _globIllPixelArray[y][x];


 /* for (size_t y=0; y<yres; y++)
  {
    for (size_t x=0; x<xres; x++)
    {
      if (show_out_of_gamut)
      {
        for (int n=0; n<resPixelArray[y][x].numSamples; n++)
        {
          if (resPixelArray[y][x][n] > 1.0f)
          {
            resPixelArray[y][x] = RED;
            break;
          }
        }
      }
      else
      {
        for (int n=0; n<resPixelArray[y][x].numSamples; n++)
        {
          if (resPixelArray[y][x][n] > 1.0f)
          {
            compMax = resPixelArray[y][x][n];
            for (int m=0; m<resPixelArray[y][x].numSamples; m++)
              resPixelArray[y][x][m] /= compMax;
            break;
          }
        }
      }
    }
  }*/

  //PPM::writeFile(xres, yres, 255, _pixelArray);
  //wchar_t filename[256];
  //mbstowcs_s(NULL, filename, options.outfile.c_str(), options.outfile.size() );
  IMAGEIO::writeImage(Options::getInstance().outfile.c_str(), xres, yres, 
                      _resPixelArray);

}


//_____________________________________________________________________________
Film::~Film()
{

}
