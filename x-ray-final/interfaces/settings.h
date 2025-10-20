#pragma once

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <vector>

#include "spectrum.h"


// Forward Deklarationen
class Accelerator;
class Camera;
class Film;
class Filter;
class LightShape;
class Options;
class Renderer;
class UnitSampler;



#if defined DLL_EXPORT || _WINDLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif


// Die folgenden Structs dienen dazu Settings für die Lichter einzustellen,
// die nicht durch das Laden einer scene file gesetzt werden können
//_____________________________________________________________________________
struct DirectionalS
{
  DirectionalS() : intensity(1.f), castsShadow(true) {}

  float intensity;
  bool castsShadow;
};


//_____________________________________________________________________________
struct PointLightS
{
  PointLightS() : intensity(1.f), castsShadow(true) {}

  float intensity;
  bool castsShadow;
};


//_____________________________________________________________________________
struct AreaLightS
{
  AreaLightS() : intensity(5.5f), photonIntensity(5.5f), uSampler(NULL),
                 lightShape(NULL),
                 showLightShape(true), castsShadow(true), discRadius(1.f) {}

  float intensity;
  float photonIntensity;
  UnitSampler* uSampler;
  // Entweder eigenes Lightshape erzeugen...
  LightShape* lightShape;
  bool showLightShape;
  bool castsShadow;

  // ...oder hier den Radius des Standard LightShapes (Disc) angeben
  float discRadius;
};


//_____________________________________________________________________________
class Settings
{
public:
  Settings(const Options& options);
  virtual ~Settings(void);

  Renderer*         _renderer;
  Renderer*         _globalRenderer;
  Film*             _film;
  Camera*           _camera;
  UnitSampler*      _cameraDirIllSampler;
  UnitSampler*      _cameraGlobIllSampler;
  //UnitSampler*      _areaLightSampler;
  Spectrum*         _backgroundColor;
  Accelerator*      _globalAccelerator;
  Filter*           _filter;

  std::vector<DirectionalS> _directionalSVec;
  std::vector<PointLightS>  _pointLightSVec;
  std::vector<AreaLightS>   _areaLightSVec;

};


extern "C"
{
  DECLDIR Settings* createSettings(Options& options);
}


#endif //SETTINGS_H_