
#include "stdafx.h"

//#include <ctime>

#include "sspscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
SSPScene::SSPScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
SSPScene::~SSPScene(void)
{
}


//_____________________________________________________________________________
void
SSPScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


  Spectrum outFilter, inFilter;
  outFilter[0] = 1;
  outFilter[1] = 1;
  outFilter[2] = 1;
  inFilter[0] = 0.1f;
  inFilter[1] = 0.91f;
  inFilter[2] = .1f;
  Dielectric* dielectric = 
    new Dielectric(WHITE, WHITE, 0.01, 0.1, .70, 500, 1.0f, 1.2f, inFilter,
                   outFilter, 1, 0.01f);


  Spectrum outFilter2, inFilter2;
  outFilter2[0] = 1;
  outFilter2[1] = 1;
  outFilter2[2] = 1;
  inFilter2[0] = 1;
  inFilter2[1] = 1;
  inFilter2[2] = 1;
  Dielectric* dielectric2 = 
    new Dielectric(WHITE, WHITE, 0.01, 0.1, .70, 500, 1.2f, 1.0f, inFilter2,
                   outFilter2, 1, 1);



  StratifiedS* rmirrorS = new StratifiedS(25);
  RealisticMirror* rmirror = 
    new RealisticMirror(RED, WHITE, WHITE, 0.05, 0.3, 0.5, 1000, 1, rmirrorS);
  /*RealisticMirror* rmirror = 
    new RealisticMirror(RED, WHITE, WHITE, 0.05, 0.3, 0.5, 80, 1, rmirrorS);*/


  //StratifiedS* rmirrorS2 = new StratifiedS(1);
  //RealisticMirror* rmirror2 = 
  //  new RealisticMirror(0xE3DEDB, WHITE, WHITE, 0.05, 0.3, 0.5, 1000, 1, rmirrorS2);


  Sphere* sphere = new Sphere(Point(-20, 220, 0), 20);
  sphere->setMaterial(dielectric);


  Sphere* sphere2 = new Sphere(Point(20, 210, -20), 10);
  sphere2->setMaterial(rmirror);


   // Gehe alle Shapes durch, suche nach untergrund und weise rmirror2 zu
  //for (size_t i=0; i<world.shapes.size(); i++)
  //{
  //  if (world.shapes[i]->getName() == stdstring(_T("untergrund") ) )
  //  {
  //    if (dynamic_cast<Instance*>(world.shapes[i]) != nullptr)
  //      const_cast<Shape*>(dynamic_cast<Instance*>(world.shapes[i])
  //        ->getContent()
  //      )->setMaterial(rmirror2);
  //  }
  //}
 
  
  // Luftblasen in die linke Kugel einfügen
  HammersleyS* hammersS = new HammersleyS(50);
  HemisphereS* hemisS = new HemisphereS(hammersS);
  std::vector<Point> hemisSamples;
  hemisS->createSamples(hemisSamples);
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distDistrib(2.0, 18.0);
  std::uniform_real_distribution<double> radDistrib(0.5, 0.8);

  for (int i=0; i<hemisSamples.size(); i++)
  {
    Sphere* tmpSphere = 
      new Sphere(hemisSamples[i] * distDistrib(generator) + Vector(-20, 220, 0), 
                 radDistrib(generator) );
    tmpSphere->setMaterial(dielectric2);
    world.addShape(tmpSphere);
  }


  


  world.addShape(sphere);
  world.addShape(sphere2);
}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new SSPScene(options);
}