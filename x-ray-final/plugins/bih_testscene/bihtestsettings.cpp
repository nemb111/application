#include "stdafx.h"


#include "bihtestsettings.h"
//#include "accelrenderer.h"
//#include "bvhwrapper.h"
//#include "simplefilm.h"
//#include "midpoints.h"
//#include "nofilter.h"
//#include "options.h"
//#include "displayinfo.h"

//_____________________________________________________________________________
BIHTestSettings::BIHTestSettings(Options& options)
  : Settings(options)
{
  _renderer = new AccelRenderer(30);
  //_renderer = new AccelRenderer(2);
  //_renderer = new ADPrenderer(2);
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeMM() / 100., 1, 0, false);
  //_camera = new Pinhole;
  _cameraDirIllSampler = new MidpointS;
  //_cameraDirIllSampler = new StratifiedS(9);
  //_cameraDirIllSampler = new HammersleyS(9);
  _backgroundColor = new Spectrum(BLACK);
  _globalAccelerator = new BIH;
  //_globalAccelerator = new BVHwrapper;
  //_globalAccelerator = new NoAccel;
  //_filter = new MitchellFilter(3, 3);
  _filter = new NoFilter;

 /* DirectionalS dirS;
  dirS.intensity = 2.f;
  _directionalSVec.push_back(dirS);*/

 /* AreaLightS areaS;
  areaS.intensity = 60000*75;
  areaS.discRadius= 0.1;
  areaS.uSampler = new StratifiedS(25);
  areaS.showLightShape = true;
  _areaLightSVec.push_back(areaS);*/

  //AreaLightS areaS2;
  //areaS2.intensity = 15000;
  //areaS2.intensity = 0;
  //areaS2.discRadius= 0.1;
  //areaS2.uSampler = new StratifiedS(9);
  //areaS2.showLightShape = true;
  //_areaLightSVec.push_back(areaS2);

}


//_____________________________________________________________________________
BIHTestSettings::~BIHTestSettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new BIHTestSettings(options);
}