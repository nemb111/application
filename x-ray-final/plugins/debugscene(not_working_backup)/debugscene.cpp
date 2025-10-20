#include "stdafx.h"


//#include <ctime>


//Abgeleitete Komponenten für die Welt
//#include "arealight.h"
//#include "accelerator.h"
//#include "accelrenderer.h"
//#include "debugsphere.h"
//#include "dielectric.h"
//#include "directional.h"
//#include "boxfilter.h"
//#include "bvhwrapper.h"
//#include "luboxfilter.h"
//#include "lightshape.h"
//#include "emissive.h"
//#include "mitchellfilter.h"
//#include "noaccel.h"
//#include "pointlight.h"
//#include "adprenderer.h"
//#include "flatmeshtriangle.h"
//#include "smoothmeshtriangle.h"
//#include "disc.h"
//#include "plane.h"
//#include "rect.h"
//#include "sphere.h"
//#include "triangle.h"
//#include "orthocamera.h"
//#include "pinhole.h"
//#include "realisticmirror.h"
//#include "simplefilm.h"
//#include "simplerenderer.h"
//#include "stratifieds.h"
//#include "transparent.h"
//#include "hammersleys.h"
//#include "midpoints.h"
//#include "matte.h"
//#include "mirror.h"
//#include "phong.h"
//#include "parser.h"
//
//
//#include "bih.h"
//#include "instance.h"
//#include "options.h"
//#include "debugscene.h"



//_____________________________________________________________________________
DebugScene::DebugScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
DebugScene::~DebugScene(void)
{
}


//_____________________________________________________________________________
void
DebugScene::add(World& world)
{

  // Hier wird die globale Option, die den Filennamen trägt verändert. Dies
  // sollte außer für Debugzwecke nicht getan werden.
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");
  

  Sphere* sphere = new Sphere(Point(0,0,0), 10);
  SmoothMeshTriangle* mesh2 = new SmoothMeshTriangle(_T("files/Pedestal/pedestal.obj"), &BVHwrapper() );
  mesh2->setThrowsShadow(false);
  Instance* instance = new Instance(mesh2);
  instance->scale(4, 4, 4);
  //instance->rotateX(PI/4);
  Spectrum filter;
  filter[0] = filter[1] = filter[2] = 1.0f;
  Dielectric* dielectric = 
    new Dielectric(YELLOW, YELLOW, 0.01, 0.3, .60, 500, 1.f, 1.35f, filter, filter);
  Transparent* transparent = 
    new Transparent(WHITE, WHITE, 0.0, 0., 0.0, 10, WHITE, 0.0f, 1.0f, 1.35f);
  Phong* phong3 = new Phong(RED,WHITE,0.05,1.,1., 50);
  mesh2->setMaterial(dielectric);
  world.addShape(instance);

   
  //Pinhole* camera = new Pinhole;
  //camera->setPos(Point(-900, 300, 100) );
  //camera->lookAt(Point(0, 100, 0) );
  //camera->setFilm(world.film);
  //camera->setUnitSampler(world.cameraSampler);
  //world.addCamera(camera);

/*
  LightShape* rectLight = new Rect(Point(2,6,-2), Vector(0,0,4), Vector(-4,0,0),
                                   Normal(0,-1,0) );
  LightShape* discLight = new Disc(Point(30,200,250), 1, Normal(-1,-1,-1) );
  LightShape* triLight = new Triangle(Point(2,6,2), Point(-2,6,2), Point(0,6,-2) );
  Emissive* emissive = new Emissive(10, WHITE);
  UnitSampler* uSampler = new StratifiedS(49);*/
  //AreaLight* aLight = new AreaLight(discLight, emissive, uSampler, world );

  //world.addLight(aLight);


  /*UnitSampler* uSampler2 = new StratifiedS(121);
  LightShape* discLight2 = new Disc(Point(4,6,-4), 1, Normal(-1,-1,1) );
  Emissive* emissive2 = new Emissive(5, WHITE);*/
  //AreaLight* aLight2 = new AreaLight(discLight2, emissive2, uSampler2, world);

  //world.addLight(aLight2);

  //Directional* dirLight = new Directional(WHITE, Vector(0.f,0.f,1.f), 2 );
  //world.addLight(dirLight);

  //Directional* dirLight2 = new Directional(WHITE, Vector(-1.f,0.f,0.f), 1.7f);
  //world.addLight(dirLight2);

  //Directional* dirLight3 = new Directional(WHITE, Vector(0.f,0.f,-1.f), 2 );
  //world.addLight(dirLight3);

  //PointLight* pointLight = new PointLight(Point(0, 650, 10) );
  //pointLight->showSphere(world, new DebugSphere, WHITE);
  //world.addLight(pointLight);


  //GBin.mrkDel<Camera*>(camera);
 /* GBin.mrkDel<Light*>(dirLight2);
  GBin.mrkDel<Shape*>(rectLight);
  GBin.mrkDel<Shape*>(discLight);
  GBin.mrkDel<Shape*>(triLight);
  GBin.mrkDel<Material*>(emissive);
  GBin.mrkDel<UnitSampler*>(uSampler);*/
  //GBin.mrkDel<Light*>(aLight);

  /*GBin.mrkDel<UnitSampler*>(uSampler2);
  GBin.mrkDel<Shape*>(discLight2);
  GBin.mrkDel<Material*>(emissive2);*/
  //GBin.mrkDel<Light*>(aLight2);






  



  //Rect* obstacle = new Rect(Point(-.5,0,.5), Vector(1,0,0), Vector(0,0,-1),
  //                          Normal(0,1,0) );
  //Matte* matte1 = new Matte(RED, 0.05, 1);
  //obstacle->setMaterial(matte1);
  //world.addShape(obstacle);


  //Rect* plane = new Rect(Point(-20, -4, 20), Vector(40, 0, 0), Vector(0,0,-40),
  //                       Normal(0,1,0) );
  ///* Matte* matte2 = new Matte(GREEN, 0.05, 1);
  //plane->setMaterial(matte2);*/
  //Mirror* mirror = new Mirror(0xFFF5EE,WHITE,WHITE,0.05,.7,0.2,500,.7);
  //StratifiedS* hammer = new StratifiedS(136);
  //RealisticMirror* rmirror = new RealisticMirror(0xFFF5EE,WHITE,WHITE,0.05,.7,
  //                                               0.2,50,.7, hammer);
  //plane->setMaterial(rmirror);
  //world.addShape(plane);

  


  //SmoothMeshTriangle* mesh = new SmoothMeshTriangle("files\\buddha.obj", &BVHwrapper() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  //Phong* phong2 = new Phong(GREEN,WHITE,0.05,1.,1., 50);
  ////Mirror* mirror2 = new Mirror(GREEN,WHITE,WHITE,0.05,.6,1.,50,1.);
  //mesh->setMaterial(phong2);
  //Instance* instance5 = new Instance(mesh);
  //instance5->scale(4, 4, 4);
  //instance5->rotateX(PI/2.);
  //instance5->translate(-3, 0, -1);
  //world.addShape(instance5);

  //SmoothMeshTriangle* mesh2 = new SmoothMeshTriangle("files\\dragon.obj", &BVHwrapper() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  //Phong* phong3 = new Phong(RED,WHITE,0.05,1.,1., 50);
  ////Mirror* mirror2 = new Mirror(GREEN,WHITE,WHITE,0.05,.6,1.,50,1.);
  //mesh2->setMaterial(phong3);
  //Instance* instance6 = new Instance(mesh2);
  //instance6->scale(6, 6, 6);
  //instance6->rotateX(PI/2.);
  //instance6->translate(5, 0, 0);
  //world.addShape(instance6);

  //SmoothMeshTriangle* mesh = new SmoothMeshTriangle("files\\buddha.obj", &BVHwrapper() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  //Phong* phong2 = new Phong(GREEN,WHITE,0.05,1.,1., 50);
  ////Mirror* mirror2 = new Mirror(GREEN,WHITE,WHITE,0.05,.6,1.,50,1.);
  //mesh->setMaterial(phong2);
  //Instance* instance5 = new Instance(mesh);
  //instance5->scale(4, 4, 4);
  //instance5->rotateX(PI/2.);
  //instance5->translate(0, 0, -1);
  //world.addShape(instance5);

  //Sphere* sphere = new Sphere(Point(0, 0, 0), 1.2);
  //Matte* matte = new Matte(GREEN,0.05,0.9);
  //Phong* phong = new Phong(BLUE,WHITE,0.25,1.,1.,30);
  //Mirror* mirror = new Mirror(YELLOW, WHITE, WHITE, 0.05, .6, 1., 30, 1.);
  //sphere->setMaterial(mirror);

  /*for (int x=0,y=0,z=0; ; x++)
  {
   
    if (x==3)
    {
      x=0;
      y++;
    }
    if (y==3)
    {
      y=0;
      z++;
    }
    if (z>2) break;
    Instance* instance = new Instance(sphere);
    instance->translate(-2 +4.*x, -2 +4.*y, 2 -4.*z);
    instance->translate(-1, 0, -5);
    world.addShape(instance);
  }*/


  //Matte* matte = new Matte(GREEN,0.05,0.9);
  //Disc* disc = new Disc(Point(0,0,0), 2.54f, Normal(1,1,1) );
  //Instance* inst = new Instance(disc);
  //disc->setMaterial(matte);
  //world.addShape(inst);

  //Rect* rectangle = new Rect(Point(0, 0, 0), Vector(40,0,0), Vector(0,0,-50),
  //                           Normal(0,1,0) );
  //Matte* matte2 = new Matte(0x9C4823, 0.05, 0.9);
  //rectangle->setMaterial(matte2);
  //Instance* instance2 = new Instance(rectangle);
  ////instance2->rotateX(PI/8.);
  //instance2->translate(-20, -6., 25);
  //world.addShape(instance2);

 /* Triangle* tri = new Triangle();
  Matte* matte3 = new Matte(RED, 0.05, 0.9);
  tri->setMaterial(matte3);
  Instance* instance3 = new Instance(tri);
  instance3->translate(0,0,0);
  instance3->scale(5,5,5);
  world.addShape(instance3);

  Rect* occl = new Rect(Point(0,0,0), Vector(2,0,0), Vector(0,1,0),
                        Normal(0,0,1) );
  Matte* matte4 = new Matte(RED, 0.05, 0.9);
  occl->setMaterial(matte4);
  Instance* instance4 = new Instance(occl);
  world.addShape(instance4);*/

  //SmoothMeshTriangle* mesh = new SmoothMeshTriangle("files\\buddha.obj", &BIH() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  ////Phong* phong2 = new Phong(GREEN,WHITE,0.05,1.,1., 50);
  //Mirror* mirror2 = new Mirror(GREEN,WHITE,WHITE,0.05,.6,1.,50,1.);
  //mesh->setMaterial(mirror2);
  //Instance* instance5 = new Instance(mesh);
  //instance5->scale(8, 8, 8);
  //instance5->rotateX(PI/2.);
  //instance5->translate(0, -1, 0);
  //world.addShape(instance5);

 
  //SmoothMeshTriangle* mesh2 = dynamic_cast<SmoothMeshTriangle*>(mesh->clone() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  ////Phong* phong3 = new Phong(RED,WHITE,0.05,1.,1., 50);
  //Mirror* mirror3 = new Mirror(RED,WHITE,WHITE,0.05,0.6,1.,50,1.);
  //mesh2->setMaterial(mirror3);
  //Instance* instance6 = new Instance(mesh2);
  //instance6->scale(4, 4, 4);
  //instance6->rotateX(PI/2.);
  //instance6->translate(5, -1, 0);
  //world.addShape(instance6);


  //SmoothMeshTriangle* mesh3 = dynamic_cast<SmoothMeshTriangle*>(mesh->clone() );
  ////mesh->switchNormal(true);
  ////Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  ////mesh->setMaterial(matte5);
  ////Phong* phong4 = new Phong(BLUE,WHITE,0.05,1.,1, 50);
  //Mirror* mirror4 = new Mirror(BLUE,WHITE,WHITE,0.05,.6,1.,50,1.);
  //mesh3->setMaterial(mirror4);
  //Instance* instance7 = new Instance(mesh3);
  //instance7->scale(4, 4, 4);
  //instance7->rotateX(PI/2.);
  //instance7->translate(-5, -1, 0);
  //world.addShape(instance7);

  //SmoothMeshTriangle* mesh2 = new SmoothMeshTriangle("files\\bunny.obj", accelerator);
  ////mesh->switchNormal(true);
  ////Matte* matte6 = new Matte(YELLOW, 0.05, 2);
  //Phong* phong9 = new Phong(GREEN,WHITE,0.05,1.,1.,50);
  //mesh2->setMaterial(phong9);
  ////mesh2->switchNormal(true);
  //Instance* instance6 = new Instance(mesh2);
  //instance6->scale(4, 4, 4);
  //instance6->translate(0, -4, -1);
  //world.addShape(instance6);


}


//_____________________________________________________________________________
bool
DebugScene::init(World& world)
{
  return true;
}


//_____________________________________________________________________________
void
DebugScene::destroyWorld()
{
}