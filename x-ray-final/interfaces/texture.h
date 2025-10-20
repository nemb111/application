/* Das Interface in dieser Datei oder der zugehörigen cpp-Datei orientieren sich an:
   Dem Buch "Ray Tracing from the ground up" / Seite 645 ff.
*/


#pragma once

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "spectrum.h"


//Forward Deklarationen
class ShadeRec;

struct UV
{
  UV() : u(0), v(0) {}
  float u;
  float v;
};


class Texture
{
public:
  Texture(void);
  virtual ~Texture(void);

  virtual RGBASpectrum getSpectrum(const ShadeRec& shadeRec) const = 0;
  virtual RGBASpectrum getSpectrum(float u, float v) const = 0;

protected:


};

#endif //TEXTURE_H_