#pragma once

#ifndef IMAGETEX_H_
#define IMAGETEX_H_

#include <vector>

#include "ray.h"
#include "texture.h"



class ImageTex : public Texture
{
public:
  ImageTex(const std::string& texPath,
           const std::vector<std::vector<RGBASpectrum> >& pxlData)
    : _pxlWidth(pxlData[0].size() ), _pxlHeight(pxlData.size() ),
      _texPath(texPath)
  {
    _pxlData.resize(pxlData.size() );
    for (size_t y=0; y<pxlData.size(); y++)
    {
      _pxlData[y].resize(pxlData[y].size() );
      for (size_t x=0; x<pxlData[y].size(); x++)
      {
        _pxlData[y][x].a = static_cast<unsigned char>(pxlData[y][x][0] * 255);
        _pxlData[y][x].b = static_cast<unsigned char>(pxlData[y][x][1] * 255);
        _pxlData[y][x].g = static_cast<unsigned char>(pxlData[y][x][2] * 255);
        _pxlData[y][x].r = static_cast<unsigned char>(pxlData[y][x][3] * 255);
      }
    }
  }
  virtual ~ImageTex(void);

  virtual RGBASpectrum getSpectrum(const ShadeRec& shadeRec) const;
  virtual RGBASpectrum getSpectrum(float u, float v) const;

protected:
  size_t _pxlWidth;
  size_t _pxlHeight;
  std::string _texPath;

  struct RGBA
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
  };
  // Im unsigned char array steht die Pixelfarbe als RGBA
  std::vector<std::vector<RGBA> > _pxlData;



};


#endif // IMAGETEX_H_