
#include "stdafx.h"

//#include <ctime>

#include "cbsscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
CBSScene::CBSScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
CBSScene::~CBSScene(void)
{
}


//_____________________________________________________________________________
void
CBSScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


  Sphere* sphereBL = new Sphere(Point(127, 100, -382), 100);
  /*Matte* matteSphereBL = new Matte(BLUE, 0.05, 0.5);
  sphereBL->setMaterial(matteSphereBL);*/
  StratifiedS* strat = new StratifiedS(9);
  RealisticMirror* rmirror = new RealisticMirror(0xE3DEDB,WHITE,WHITE,0.05,.2,
                                                 0.0,1000000,.8, strat);
  sphereBL->setMaterial(rmirror);
  Mirror* mirror = new Mirror(0xE3DEDB,WHITE,WHITE,0.05,.2, 0.0,10,.8);
  /*sphereBL->setMaterial(mirror);*/
 
  


  Sphere* sphereFR = new Sphere(Point(427, 100, -158), 100);
  //Sphere* sphereFR = new Sphere(Point(427/2., 100, -158), 100);
  /*Matte* matteSphereFR = new Matte(YELLOW, 0.05, 0.5);
  sphereFR->setMaterial(matteSphereFR);*/
  /*Transparent* transparent = new Transparent(WHITE, BLACK, 0.01, 0.1, .6,
                                             500, WHITE, 0.01, 0.99, 1.5);
  sphereFR->setMaterial(transparent);*/
  Spectrum outFilter, inFilter;
  outFilter[0] = outFilter[1] = outFilter[2] = 1.f;
  inFilter[0] = inFilter[1] = inFilter[2] = 1.0f;
  Dielectric* dieSphereFR = 
    new Dielectric(WHITE, WHITE, 0.01, 0.1, .60, 500, 1.0f, 1.3f, inFilter,
                   outFilter, 1.f, 0.0009f);
  sphereFR->setMaterial(dieSphereFR);




  //world.addShape(sphereBL);
  //world.addShape(sphereFR);
 

  // Gehe alle Shapes durch, suche nach Box001 und weise mirror zu
  //for (size_t i=0; i<world.shapes.size(); i++)
  //{
  //  if (world.shapes[i]->getName() == stdstring(_T("Box001") ) )
  //  {
  //    if (dynamic_cast<Instance*>(world.shapes[i]) != nullptr)
  //      const_cast<Shape*>(dynamic_cast<Instance*>(world.shapes[i])
  //        ->getContent()
  //      )->setMaterial(rmirror);
  //  }
  //}


  UnitSampler* uSampler = /*new MidpointS;*/ new StratifiedS(16);
  Emissive* emissive = new Emissive;
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
  return new CBSScene(options);
}