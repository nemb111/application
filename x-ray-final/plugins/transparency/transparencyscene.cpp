
#include "stdafx.h"
//#include "x-ray_includes.h"
#include "transparencyscene.h"


//_____________________________________________________________________________
TransparencyScene::TransparencyScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
TransparencyScene::~TransparencyScene(void)
{
}


//_____________________________________________________________________________
void
TransparencyScene::add(World& world)
{

  Emissive* emissiveMat = new Emissive;
  HammersleyS* hammersleyS = new HammersleyS(10);
  EnvironmentLight* environmentLight = new EnvironmentLight(2.0f, WHITE,
                                                            hammersleyS);
  world.addLight(environmentLight);


  Sphere* sphere = new Sphere(Point(0,0,0), 10);
  SmoothMeshTriangle* mesh2 = new SmoothMeshTriangle(_T("files/transparency/horse.obj"), &BVHwrapper() );
  mesh2->setThrowsShadow(false);
  Instance* instance = new Instance(mesh2);
  instance->scale(0.8, 0.8, 0.8);
  //instance->rotateX(PI/4);
  Spectrum filter;
  filter[0] = filter[1] = filter[2] = 0.85f;
  Dielectric* dielectric = 
    new Dielectric(BLACK, WHITE, 0.01, 0.3, .60, 500, 1.f, 1.05f, filter,
                   filter, 1.f, 0.017f);
  Transparent* transparent = 
    new Transparent(WHITE, WHITE, 0.0, 0., 0.0, 10, WHITE, 0.0f, 1.0f, 1.35f);
  Phong* phong = new Phong(RED,WHITE,0.05,1.,1., 50);
  mesh2->setMaterial(dielectric);
  world.addShape(instance);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new TransparencyScene(options);
}