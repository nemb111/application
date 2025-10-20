
#include "stdafx.h"

#include "defaultscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
DefaultScene::DefaultScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
DefaultScene::~DefaultScene(void)
{
}


//_____________________________________________________________________________
void
DefaultScene::add(World& world)
{

  Emissive* emissiveMat = new Emissive;
  StratifiedS* stratS = new StratifiedS(256);
  EnvironmentLight* environmentLight = new EnvironmentLight(4.0f, WHITE,
                                                            stratS);
  world.addLight(environmentLight);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new DefaultScene(options);
}