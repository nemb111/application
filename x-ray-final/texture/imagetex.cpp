#include "stdafx.h"


//_____________________________________________________________________________
ImageTex::~ImageTex(void)
{
}


//_____________________________________________________________________________
RGBASpectrum
ImageTex::getSpectrum(const ShadeRec& shadeRec) const
{
  size_t xIndex = static_cast<size_t>((_pxlWidth-1) * shadeRec.uv.u);
  size_t yIndex = static_cast<size_t>((_pxlHeight-1) * shadeRec.uv.v);

  RGBASpectrum rgbaSpectrum;
  rgbaSpectrum[0] = _pxlData[yIndex][xIndex].a / 255.f;
  rgbaSpectrum[1] = _pxlData[yIndex][xIndex].b / 255.f;
  rgbaSpectrum[2] = _pxlData[yIndex][xIndex].g / 255.f;
  rgbaSpectrum[3] = _pxlData[yIndex][xIndex].r / 255.f;


  return rgbaSpectrum;
}


//_____________________________________________________________________________
RGBASpectrum
ImageTex::getSpectrum(float u, float v) const
{
  size_t xIndex = static_cast<size_t>((_pxlWidth-1) * u);
  size_t yIndex = static_cast<size_t>((_pxlHeight-1) * v);

  RGBASpectrum rgbaSpectrum;
  rgbaSpectrum[0] = _pxlData[yIndex][xIndex].a / 255.f;
  rgbaSpectrum[1] = _pxlData[yIndex][xIndex].b / 255.f;
  rgbaSpectrum[2] = _pxlData[yIndex][xIndex].g / 255.f;
  rgbaSpectrum[3] = _pxlData[yIndex][xIndex].r / 255.f;


  return rgbaSpectrum;
}