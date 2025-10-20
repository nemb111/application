
#include "stdafx.h"

//#include <ctime>

//#include "x-ray_includes.h"
#include "transparencysettings.h"



//_____________________________________________________________________________
TransparencySettings::TransparencySettings(Options& options)
  : Settings(options)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");

    _renderer = new AccelRenderer(30);
  //_renderer = new AccelRenderer(2);
  //_renderer = new ADPrenderer(2);
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeMM() / 100., 1, 0, true);
  //_camera = new Pinhole;
  //_cameraSampler = new MidpointS;
  //_cameraSampler = new StratifiedS(9);
  _cameraDirIllSampler = new HammersleyS(4);
  _backgroundColor = new Spectrum(BLACK);
  //_globalAccelerator = new BIH;
  _globalAccelerator = new BVHwrapper;
  //_globalAccelerator = new NoAccel;
  //_filter = new MitchellFilter(3, 3);
  _filter = new NoFilter;

  DirectionalS dirS;
  dirS.intensity = 0.f;
  _directionalSVec.push_back(dirS);

}


//_____________________________________________________________________________
TransparencySettings::~TransparencySettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new TransparencySettings(options);
}