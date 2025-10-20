
#include "stdafx.h"

#include "antialiasingscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
AntialiasingScene::AntialiasingScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
AntialiasingScene::~AntialiasingScene(void)
{
}


//_____________________________________________________________________________
void
AntialiasingScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");

 /* for (size_t i=0; i<world.shapes.size(); i++)
  {
    if (world.shapes[i]->getName() == _T("drache") )
    {
      auto tmp = dynamic_cast<const Instance*>(world.shapes[i])->getContent();
      const_cast<Shape*>(tmp)->setThrowsShadow(false);
    }
  }*/

  Sphere* sphere = new Sphere(Point(10, 10, -15), 10);

  Matte* matte = new Matte(GREEN, 0.05, 0.9);
  Mirror* mirror = new Mirror(0xFFF5EE,WHITE,WHITE,0.05,.7,0.2,500,.7);
  sphere->setMaterial(mirror);

  world.addShape(sphere);


  Directional* directional = new Directional(WHITE, Vector(-.8,-.1,-1), 3 );

  world.addLight(directional);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new AntialiasingScene(options);
}