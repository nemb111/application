
#include "stdafx.h"

#include "trexscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
TrexScene::TrexScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
TrexScene::~TrexScene(void)
{
}


//_____________________________________________________________________________
void
TrexScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


 /* StratifiedS* stratS = new StratifiedS(256);
  EnvironmentLight* environmentLight = new EnvironmentLight(4.0f, WHITE,
                                                            stratS);
  world.addLight(environmentLight);*/

  Directional* directional = new Directional(WHITE, Vector(0,0,-1), 2 );

  world.addLight(directional);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new TrexScene(options);
}