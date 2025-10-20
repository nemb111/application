#include "stdafx.h"

//#include "linAlg.h"
//#include "normalmap.h"
//#include "ray.h"
//#include "texture.h"
//#include "x-ray.h"

//_____________________________________________________________________________
NormalMap::NormalMap(const Texture* normalTex)
  : _normalTex(normalTex)
{
  assert(_normalTex);
}


//_____________________________________________________________________________
NormalMap::~NormalMap(void)
{
}


//_____________________________________________________________________________
Normal
NormalMap::getNormal(const ShadeRec& sr)const
{
  const RGBASpectrum rgba = _normalTex->getSpectrum(sr);
  Spectrum xyz = rgba.getRGB();

  Normal normal((xyz[2] * -2) + 1, (xyz[1] * -2) + 1, (xyz[0] * -2) + 1);

  // Gebe die zu Weltkoordinaten transformierten Normale zurück.
  return sr.invTransMat.transpose() * normal;
}