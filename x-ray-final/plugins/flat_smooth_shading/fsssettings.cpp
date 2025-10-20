
#include "stdafx.h"

#include "fsssettings.h"
//#include "x-ray_includes.h"

bool comp(const TracePhoton& tracePhoton) 
{
  return true;
}

//_____________________________________________________________________________
FSSSettings::FSSSettings(Options& options)
  : Settings(options)
{

  _renderer = new AccelRenderer(30, 60);
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeCM(), 1, 0, false);
  //_cameraDirIllSampler = new MidpointS;
  _cameraDirIllSampler = new StratifiedS(9);
  //_cameraGlobIllSampler = new MidpointS;
  _cameraGlobIllSampler = new MidpointS;
  _backgroundColor = new Spectrum(BLACK);
  _globalAccelerator = new BVHwrapper;
  //_globalAccelerator = new BIH;
  //_globalAccelerator = new NoAccel;
  //_filter = new MitchellFilter(3, 3);
  _filter = new NoFilter;

}


//_____________________________________________________________________________
FSSSettings::~FSSSettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new FSSSettings(options);
}