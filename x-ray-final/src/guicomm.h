#pragma once

#ifndef GUICOMM_H_
#define GUICOMM_H_

#include <functional>

class SaveAndCancel;
class RGBSpectrum;
typedef RGBSpectrum Spectrum;

class GUIcomm
{
public:
  GUIcomm(void);
  ~GUIcomm(void);

  std::function<void(size_t xIdx, size_t yIdx, const Spectrum& spectrum)> updateBitmap;
  std::function<bool()> saveAndCancel;
};

#endif //GUICOMM_H_