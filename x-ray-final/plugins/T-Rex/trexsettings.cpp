#include "stdafx.h"


#include "trexsettings.h"
//#include "accelrenderer.h"
//#include "bvhwrapper.h"
//#include "simplefilm.h"
//#include "midpoints.h"
//#include "nofilter.h"
//#include "options.h"
//#include "displayinfo.h"

//_____________________________________________________________________________
TrexSettings::TrexSettings(Options& options)
  : Settings(options)
{
  _renderer = new AccelRenderer(30);
  //_renderer = new AccelRenderer(2);
  //_renderer = new ADPrenderer(2);
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeMM() / 100., 1, 0, false);
  //_camera = new Pinhole;
  //_cameraSampler = new MidpointS;
  //_cameraSampler = new StratifiedS(9);
  _cameraDirIllSampler = new HammersleyS(9);
  _backgroundColor = new Spectrum(BLACK);
  //_globalAccelerator = new BIH;
  _globalAccelerator = new BVHwrapper;
  //_globalAccelerator = new NoAccel;
  //_filter = new MitchellFilter(3, 3);
  _filter = new NoFilter;

  DirectionalS dirS;
  dirS.intensity = 2.f;
  _directionalSVec.push_back(dirS);

 /* AreaLightS areaS;
  areaS.intensity = 60000*75;
  areaS.discRadius= 0.1;
  areaS.uSampler = new StratifiedS(25);
  areaS.showLightShape = true;
  _areaLightSVec.push_back(areaS);*/

  AreaLightS areaS2;
  areaS2.intensity = 50000*75;
  areaS2.discRadius= 1;
  areaS2.uSampler = new StratifiedS(25);
  areaS2.showLightShape = true;
  _areaLightSVec.push_back(areaS2);

}


//_____________________________________________________________________________
TrexSettings::~TrexSettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new TrexSettings(options);
}