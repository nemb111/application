
#include "stdafx.h"

#include "cbhsettings.h"
//#include "x-ray_includes.h"

bool comp(const TracePhoton& tracePhoton) 
{
  return true;
}

//_____________________________________________________________________________
CBHSettings::CBHSettings(Options& options)
  : Settings(options)
{

  PREND::Params cParams;
  //cParams.disable = true;
  cParams.useRadiusSearch = true;
  cParams.knn = 100;
  cParams.radius = 20;
  cParams.filterModus = /*PRF::NO_FILTER;*/ PRF::GAUS_FILTER;
  cParams.flannSearchParams.max_neighbors = 1000;
  cParams.geom.geomType = cParams.geom.DISC;


  PREND::Params gParams;
  //gParams.disable = true;
  gParams.useRadiusSearch = true;
  gParams.knn = 500;
  gParams.radius = 50;
  gParams.flannSearchParams.max_neighbors = 2000;
  gParams.filterModus = PRF::GAUS_FILTER; /*PRF::NO_FILTER;*/
  gParams.geom.geomType = gParams.geom.CHULL;
  //gParams.geom.diskHeight = 1.9f;
  //gParams.geom.useDensEst = true;
  gParams.geom.chullHeight = 1.9f;


  _renderer = new AccelRenderer(30, 60);
  StratifiedS* photonStratS = new StratifiedS(100);
  HemisphereS* lightHemisS = new HemisphereS(photonStratS, 1.);
  _globalRenderer = new PhotonRenderer(0, 500000, 20, lightHemisS);
  //dynamic_cast<PhotonRenderer*>(_globalRenderer)->debugCaustics(true, 1.f, comp);
  //dynamic_cast<PhotonRenderer*>(_globalRenderer)->debugGlobal(true, 1.f, comp);
  dynamic_cast<PhotonRenderer*>(_globalRenderer)->setCParams(cParams);
  dynamic_cast<PhotonRenderer*>(_globalRenderer)->setGParams(gParams);
    //[](int depth) -> bool {return depth == 3;});
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeCM(), 1, 0, false);
  _cameraDirIllSampler = new MidpointS;
  //_cameraDirIllSampler = new StratifiedS(9);
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
CBHSettings::~CBHSettings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new CBHSettings(options);
}