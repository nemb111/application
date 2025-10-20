
#include "stdafx.h"

//#include <ctime>

#include "fssscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
FSSScene::FSSScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
FSSScene::~FSSScene(void)
{
}


//_____________________________________________________________________________
void
FSSScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");

  FlatMeshTriangle* fmt = new FlatMeshTriangle(_T("./files/flat_smooth_shading/bunny_simplified.obj"), &BVHwrapper() );
  SmoothMeshTriangle* smt = new SmoothMeshTriangle(_T("./files/flat_smooth_shading/bunny_simplified.obj"), &BVHwrapper() );
  fmt->setThrowsShadow(false);
  smt->setThrowsShadow(false);
  

  Phong* phong = new Phong(GREEN,WHITE,0.05,.6,0.1,40);
  fmt->setMaterial(phong);
  smt->setMaterial(phong);

  Instance* instance = new Instance(fmt);
  //Instance* instance = new Instance(smt);
  instance->scale(3.1, 3.1, 3.1);
  instance->translate(0, .6, 0);


  UnitSampler* uSampler = /*new MidpointS;*/ new StratifiedS(16);
  Emissive* emissive = new Emissive;
  LightShape* lightShape = new Rect(Point(-2, -2, 30), Vector(0,4,0),
                                    Vector(4,0,0), Normal(0,0,-1) );
  /*LightShape* lightShape = new Disc(Point(0, -10, 0), 1, Normal(0,1,0) );*/
  AreaLight* areaLight = new AreaLight(40000, 0, WHITE, lightShape,
                                       emissive, uSampler, world);

  world.addLight(areaLight);
  world.addShape(instance);
}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new FSSScene(options);
}