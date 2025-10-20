#include "stdafx.h"


//#include "options.h"
//#include "renderer.h"
//#include "settings.h"



//_____________________________________________________________________________
Settings::Settings(const Options& options)
  : _renderer(NULL), _globalRenderer(nullptr), _film(NULL), _camera(NULL),
    _cameraDirIllSampler(NULL), _cameraGlobIllSampler(NULL),
    /*_areaLightSampler(NULL),*/ _backgroundColor(NULL), _globalAccelerator(NULL),
    _filter(NULL)
{

  // Die DLL wird zur Laufzeit geladen und kennt daher nicht die bis dahin
  // erzeugten statischen Variablen. Diese werden deshalb per Konstruktur
  // übergeben und erneut gesetzt.
  Options::getInstance() = options;
}


//_____________________________________________________________________________
Settings::~Settings(void)
{
  delete _renderer;
  _renderer = 0;
  delete _film;
  _film = 0;
  delete _camera;
  _camera = 0;
  delete _cameraDirIllSampler;
  _cameraDirIllSampler = 0;
  delete _cameraGlobIllSampler;
  _cameraGlobIllSampler = 0;
  delete _backgroundColor;
  _backgroundColor = 0;
  delete _globalAccelerator;
  _globalAccelerator = 0;
  delete _filter;
  _filter = 0;
}
