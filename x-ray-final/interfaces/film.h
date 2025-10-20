/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 272ff.
*/


#pragma once

#ifndef FILM_H_
#define FILM_H_

#include <vector>

#include "filter.h"
#include "guicomm.h"
#include "spectrum.h"
#include "x-ray.h"


typedef std::vector<std::vector<Spectrum> > Spectrum2D;

class Film
{
public:
  
  virtual ~Film();

  uint getPxlWidth() const {return xres;}
  uint getPxlHeight() const {return yres;}
  double getPxlSize() const {return pixelSize;}

  
  void filter(Filter* filter, GUIcomm* guicomm);

  //der Index startet wie gewohnt mit 0, d.h. der oberste linke Pixel
  //wird mit x=0 & y=0 adressiert
  virtual const Spectrum& getGlobIllPixelSpectrum(int x, int y) const;
  virtual const Spectrum& getResPixelSpectrum(int x, int y) const;
  virtual void setDirIllPixelSpectrum(int x, int y, const Spectrum& s);
  virtual void setGlobIllPixelSpectrum(int x, int y, const Spectrum& s);
  virtual void writeImage();


protected:
  Film(int xr, int yr, double pSize, float gam, float inv_gam,
       bool s_o_o_g);

  virtual void updateResPixelArray(int x, int y);

  
  uint 			xres;   					// horizontal image resolution 
	uint 			yres;   					// vertical image resolution
	double	  pixelSize;				// pixel size
	//int				nSamples;	  			// number of samples per pixel
		
	float			gamma;						// gamma correction factor
	float			inv_gamma;					// the inverse of the gamma correction factor
  float     _invHighestCoeff; // the highest Spectrum coefficient recorded
                              // used for normalizing _resPixelArray;
	bool			show_out_of_gamut;			// display red if RGBColor out of gamut
  //std::vector<std::vector<bool> > _resPixelArraySet;
  Spectrum2D _resPixelArray;
  Spectrum2D _dirIllPixelArray;
  Spectrum2D _globIllPixelArray;

};


#endif//FILM_H_

