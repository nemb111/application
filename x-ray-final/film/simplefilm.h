#pragma once

#ifndef SIMPLEFILM_H_
#define SIMPLEFILM_H_

#include "film.h"
class SimpleFilm : public Film
{
public:
  SimpleFilm();
  SimpleFilm(int xres, int yres, double pixelSize, float gamma,
             float inv_gamma, bool show_out_of_gamut);
  virtual ~SimpleFilm();
};

#endif//SIMPLEFILM_H_