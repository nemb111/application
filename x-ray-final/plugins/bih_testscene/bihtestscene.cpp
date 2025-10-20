
#include "stdafx.h"

#include "bihtestscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
BIHTestscene::BIHTestscene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
BIHTestscene::~BIHTestscene(void)
{
}


//_____________________________________________________________________________
void
BIHTestscene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


  Pinhole* pinhole = new Pinhole(Point(0,0,100), Normal(0,0,-1), Normal(0,1,0),
                                 2);
  pinhole->setFilm(world.film);
  pinhole->setUnitSampler(world.cameraDirIllSampler);
  world.addCamera(pinhole);


  Sphere* sphere1 = new Sphere(Point(-2, -2, -2), 2);
  Sphere* sphere2 = new Sphere(Point(-4, -2, 1),  1);
  Sphere* sphere3 = new Sphere(Point(0, 1, 0), 2);
  Sphere* sphere4 = new Sphere(Point(1, 2, 3), 2);
  Sphere* sphere5 = new Sphere(Point(4, 3, 1), 1);
  Sphere* sphere6 = new Sphere(Point(0, -3, 0), 3);
  Sphere* sphere7 = new Sphere(Point(0, 0, 0), 2);

  Matte* matte1 = new Matte(GREEN, 0.05, 0.9);
  Matte* matte2 = new Matte(RED, 0.05, 0.9);
  Matte* matte3 = new Matte(BLUE, 0.05, 0.9);

  sphere1->setMaterial(matte1);
  sphere2->setMaterial(matte1);
  sphere3->setMaterial(matte1);
  sphere4->setMaterial(matte2);
  sphere5->setMaterial(matte2);
  sphere6->setMaterial(matte3);
  sphere7->setMaterial(matte3);

  world.addShape(sphere1);
  world.addShape(sphere2);
  world.addShape(sphere3);
  world.addShape(sphere4);
  world.addShape(sphere5);
  world.addShape(sphere6);
  world.addShape(sphere7);



  Directional* directional = new Directional(WHITE, Vector(0,0,-1), 2 );

  world.addLight(directional);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new BIHTestscene(options);
}