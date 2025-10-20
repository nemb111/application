#include "stdafx.h"

//Abgeleitete Komponenten für die Welt
//#include "debugsphere.h"
//#include "directional.h"
//#include "pointlight.h"
//#include "adprenderer.h"
//#include "flatmeshtriangle.h"
//#include "plane.h"
//#include "rect.h"
//#include "sphere.h"
//#include "triangle.h"
//#include "orthocamera.h"
//#include "pinhole.h"
//#include "simplefilm.h"
//#include "simplerenderer.h"
//#include "matte.h"
//#include "phong.h"
//
//
//#include "instance.h"
//#include "simplescene.h"


//_____________________________________________________________________________
void
SimpleScene::add(World& world)
{
  
  //_world = new World;
  //SimpleFilm* film   = new SimpleFilm(1366, 768, 0.03, 1, 0, false);
  ////MidPntSampler* sampler = new MidPntSampler();
  //Stratified* sampler = new Stratified();
  //Pinhole* camera = new Pinhole(film, sampler);
  //Spectrum* backgroundcolor = new Spectrum(BLACK);
  //ADPrenderer* renderer = new ADPrenderer();

  //_world->film = film;
  //_world->camera = camera;
  //_world->backgroundColor = backgroundcolor;
  //_world->renderer = renderer;

  //camera->setPos(Point(3, 1, 7) );
  //camera->lookAt(Point(0, 0, 0) );
  //Directional* directional = new Directional(Spectrum(WHITE), Vector(-1,-1,-1) );
  ////_world->addLight(directional);
  //PointLight* pointlight1 = new PointLight(Point(3, 1, 6));
  //PointLight* pointlight2 = new PointLight(Point(-3, 5, 4));
  //pointlight1->showSphere(*_world);
  //pointlight2->showSphere(*_world);
  //_world->addLight(pointlight1);
  //_world->addLight(pointlight2);


  //Sphere* sphere = new Sphere(Point(0, 0, 0), 5);
  ////Matte* matte = new Matte(GREEN,0.05,0.9);
  //Phong* phong = new Phong(GREEN,WHITE,0.05,1.,1.,50);
  //sphere->setMaterial(phong);
  //Instance* instance1 = new Instance(sphere);
  //instance1->translate(0, 0, -11);
  //_world->addShape(instance1);

  //Rect* rectangle = new Rect(Point(0, 0, 0), Vector(40,0,0), Vector(0,0,-50),
  //                           Normal(0,1,0) );
  //Matte* matte2 = new Matte(0x9C4823, 0.05, 0.9);
  //rectangle->setMaterial(matte2);
  //Instance* instance2 = new Instance(rectangle);
  ////instance2->rotateX(PI/8.);
  //instance2->translate(-20, -6., 25);
  //_world->addShape(instance2);

  //Triangle* tri = new Triangle();
  //Matte* matte3 = new Matte(RED, 0.05, 0.9);
  //tri->setMaterial(matte3);
  //Instance* instance3 = new Instance(tri);
  //instance3->translate(0,0,0);
  //instance3->scale(5,5,5);
  //_world->addShape(instance3);

  //Rect* occl = new Rect(Point(-0.5,-0.5,0), Vector(1,0,0), Vector(0,1,0),
  //                      Normal(0,0,1) );
  //Matte* matte4 = new Matte(RED, 0.05, 0.9);
  //occl->setMaterial(matte4);
  //Instance* instance4 = new Instance(occl);
  ////_world->addShape(instance4);

  //FlatMeshTriangle* mesh = new FlatMeshTriangle("files\\cube.obj");
  ////mesh->switchNormal(true);
  //Matte* matte5 = new Matte(YELLOW, 0.05, 2);
  //mesh->setMaterial(matte5);
  //Instance* instance5 = new Instance(mesh);
  //instance5->scale(3, 3, 3);
  //instance5->translate(-4, 0, 1);
  //_world->addShape(instance5);

  //FlatMeshTriangle* mesh2 = new FlatMeshTriangle("files\\bunny.obj");
  ////mesh->switchNormal(true);
  //Matte* matte6 = new Matte(YELLOW, 0.05, 2);
  //mesh2->setMaterial(matte6);
  //Instance* instance6 = new Instance(mesh2);
  //instance6->scale(4, 4, 4);
  //instance6->translate(0, 0, -1);
  //_world->addShape(instance6);


}


////_____________________________________________________________________________
//bool
//SimpleScene::init(World& world)
//{
//
//  return true;
//}
//
//
////_____________________________________________________________________________
//void
//SimpleScene::destroyWorld()
//{
//  delete _world;
//  _world = 0;
//
//}