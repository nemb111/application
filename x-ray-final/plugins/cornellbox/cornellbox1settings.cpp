#include "stdafx.h"

#include "cornellbox1settings.h"
//#include "x-ray_includes.h"

bool comp(const TracePhoton& tracePhoton) 
{
  return (tracePhoton.depth == 1);

  return false;
}

//_____________________________________________________________________________
CornellBox1Settings::CornellBox1Settings(Options& options)
  : Settings(options)
{

  PREND::Params params;
  params.useRadiusSearch = true;
  params.knn = 100;
  params.radius = 60;
  //params.diskHeight = 10000;
  params.flannSearchParams.max_neighbors = 1000;
  params.filterModus = PRF::GAUS_FILTER;
  params.geom.geomType = params.geom.DISC;

  _renderer = new AccelRenderer(30, 6);
  StratifiedS* photonStratS = new StratifiedS(100);
  HemisphereS* lightHemisS = new HemisphereS(photonStratS, 1.);
  _globalRenderer = new PhotonRenderer(0, 50000, 60000, lightHemisS);
  //dynamic_cast<PhotonRenderer*>(_globalRenderer)->debugCaustics(true, 1.f, comp);
  //dynamic_cast<PhotonRenderer*>(_globalRenderer)->debugGlobal(true, 1.f, comp);
  dynamic_cast<PhotonRenderer*>(_globalRenderer)->setCParams(params);
  dynamic_cast<PhotonRenderer*>(_globalRenderer)->setGParams(params);
    //[](int depth) -> bool {return depth == 3;});
  _film   = new SimpleFilm(options.xResolution, options.yResolution,
                           getDispPxlSizeMM() / 100., 1, 0, true);
  _cameraDirIllSampler = new MidpointS; /*new StratifiedS(9);*/
  _backgroundColor = new Spectrum(BLACK);
  _globalAccelerator = new BVHwrapper;
  //_globalAccelerator = new BIH;
  //_globalAccelerator = new NoAccel;
  //_filter = new MitchellFilter(3, 3);
  _filter = new NoFilter;

}


//_____________________________________________________________________________
CornellBox1Settings::~CornellBox1Settings(void)
{
}


//_____________________________________________________________________________
Settings*
createSettings(Options& options)
{
  return new CornellBox1Settings(options);
}