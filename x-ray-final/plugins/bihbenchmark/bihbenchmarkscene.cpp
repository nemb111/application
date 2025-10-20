
#include "stdafx.h"

#include "bihbenchmarkscene.h"
//#include "x-ray_includes.h"

#define TEST 2


//_____________________________________________________________________________
BIHBenchmarkScene::BIHBenchmarkScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
BIHBenchmarkScene::~BIHBenchmarkScene(void)
{
}


//_____________________________________________________________________________
void
BIHBenchmarkScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


  Pinhole* pinhole = new Pinhole(Point(0,0,10), Normal(0,0,-1), Normal(0,1,0),
                                 world.film->getPxlWidth() *
                                 world.film->getPxlSize(),
                                 1.7453293);
  pinhole->setFilm(world.film);
  pinhole->setUnitSampler(world.cameraDirIllSampler);
  world.addCamera(pinhole);


#if TEST == 1
  const int numSpheres = 5000000;

  std::vector<Matte*> matteVec(3);
  matteVec[0] = new Matte(GREEN, 0.05, 0.9);
  matteVec[1] = new Matte(RED, 0.05, 0.9);
  matteVec[2] = new Matte(BLUE, 0.05, 0.9);
  std::vector<Sphere*> sphereVec;
  sphereVec.reserve(numSpheres);

  std::default_random_engine generator;
  std::uniform_real_distribution<double> xyDistr(-3.,3.0);
  std::uniform_real_distribution<double> zDistr(0, 6);
  std::uniform_int_distribution<int>     matDistr(0, 2);

  for (int i=0; i<numSpheres; i++)
  {
    sphereVec[i] = new Sphere(Point(xyDistr(generator), xyDistr(generator),
                                    zDistr(generator) ), .1);
    sphereVec[i]->setMaterial(matteVec[matDistr(generator)]);
    world.addShape(sphereVec[i]);
  }
#else if TEST == 2

  const int numBuddha = 1000;
  double radius = 7;

  SmoothMeshTriangle* buddha1 =
    new SmoothMeshTriangle(_T("./files/misc/buddha.obj"), world.accelerator);
  SmoothMeshTriangle* buddha2 =
    new SmoothMeshTriangle(_T("./files/misc/buddha.obj"), world.accelerator);
  SmoothMeshTriangle* buddha3 =
    new SmoothMeshTriangle(_T("./files/misc/buddha.obj"), world.accelerator);
  Phong* phong1 = new Phong(0x3333FF, WHITE, 0.05, 0.8, 0.2, 60);
  Phong* phong2 = new Phong(0xFF8000, WHITE, 0.05, 0.8, 0.2, 60);
  Phong* phong3 = new Phong(0xFF3333, WHITE, 0.05, 0.8, 0.2, 60);
  
  buddha1->setMaterial(phong1);
  buddha1->setThrowsShadow(false);
  buddha2->setMaterial(phong2);
  buddha2->setThrowsShadow(false);
  buddha3->setMaterial(phong3);
  buddha3->setThrowsShadow(false);

  Instance* instVec[numBuddha];


  std::default_random_engine generator;
  std::normal_distribution<double> normDist(0, 1);
  std::uniform_real_distribution<double> uniformDist(0, 1);
  std::uniform_int_distribution<int> uniformIntDist(0, 2);


  for (int i=0; i<numBuddha; i++)
  {
    // Erzeuge die Buddhas innerhalb eine Kugel mit Mittelpunkt (0,0,3) und
    // Radius 3
    // Formel stammt von http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
    
    double x = normDist(generator);
    double y = normDist(generator);
    double z = normDist(generator);
    double u = uniformDist(generator);

    double fraction = (radius * std::pow(u, 1./3.) ) /
      std::sqrt(x * x + y * y + z * z);

    x *= fraction;
    y *= fraction;
    z  = z * fraction - 3;

    switch (uniformIntDist(generator) )
    {
    case 0:
      instVec[i] = new Instance(buddha1);
      break;

    case 1:
      instVec[i] = new Instance(buddha2);
      break;

    case 2:
      instVec[i] = new Instance(buddha3);
      break;
    }
    
    instVec[i]->rotateX(PI / 2.);
    instVec[i]->translate(x, y, z);

    world.addShape(instVec[i]);

  }



#endif // TEST


  Directional* directional = new Directional(WHITE, Vector(0,0,-1), 2 );

  world.addLight(directional);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new BIHBenchmarkScene(options);
}