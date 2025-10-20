
#include "stdafx.h"
/*#include <ctime>

#include "x-ray_includes.h"*/
#include "cornellbox1scene.h"



//_____________________________________________________________________________
CornellBox1Scene::CornellBox1Scene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
CornellBox1Scene::~CornellBox1Scene(void)
{
}


//_____________________________________________________________________________
void
CornellBox1Scene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");

  //Emissive* emissiveMat = new Emissive(4.0f, WHITE);
  //StratifiedS* stradS = new StratifiedS(10);
  //EnvironmentLight* environmentLight = new EnvironmentLight(emissiveMat,
  //                                                          stradS);
  //world.addLight(environmentLight);


 /* Directional* directional = new Directional(WHITE, Vector(0,-1,0), 2);
  world.addLight(directional);
*/
  //Disc* disc = new Disc(Point(278, 273,650), 10, Normal(0,0,1) );
  //Matte* matte = new Matte(GREEN, .05f, 1);
  //disc->setMaterial(matte);

  //world.addShape(disc);

  //Rect* rect = new Rect(Point(268, 263, 650), Vector(20, 0, 0), Vector(0, 20, 0),
  //                      Normal(0,0,1) );
  //Matte* matte = new Matte(GREEN, .05f, 1);
  //rect->setMaterial(matte);

  //world.addShape(rect);


  UnitSampler* uSampler = new StratifiedS(16);
  Emissive* emissive = new Emissive;
  //LightShape* lightShape2 = new Disc(Point(263, 600, -277), 50, Normal(0,-1,0) );
  LightShape* lightShape = new Rect(Point(213, 548.8, -332), Vector(130,0,0),
                                    Vector(0,0,105), Normal(0,-1,0) );
  AreaLight* areaLight = new AreaLight(6000000000, 30000000, WHITE, lightShape,
                                       emissive, uSampler, world);

  world.addLight(areaLight);
}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new CornellBox1Scene(options);
}