#include "stdafx.h"


#include "defaultsettings.h"
//#include "accelrenderer.h"
//#include "bvhwrapper.h"
//#include "simplefilm.h"
//#include "midpoints.h"
//#include "nofilter.h"
//#include "options.h"
//#include "displayinfo.h"

//_____________________________________________________________________________
DefaultSettings::DefaultSettings(Options& options)
  : Settings(options)
{
   _renderer = new AccelRenderer(1);
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeMM() / 100., 1, 0, true);
  
  _cameraDirIllSampler = new MidpointS;
  _backgroundColor = new Spectrum(BLACK);
  _globalAccelerator = new BVHwrapper;
  _filter = new NoFilter;

  DirectionalS dirS;
  dirS.intensity = 0.f;
  _directionalSVec.push_back(dirS);

}


//_____________________________________________________________________________
DefaultSettings::~DefaultSettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new DefaultSettings(options);
}