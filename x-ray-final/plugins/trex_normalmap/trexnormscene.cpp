
#include "stdafx.h"

#include "trexnormscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
TrexNormScene::TrexNormScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
TrexNormScene::~TrexNormScene(void)
{
}


//_____________________________________________________________________________
void
TrexNormScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


 /* StratifiedS* stratS = new StratifiedS(256);
  EnvironmentLight* environmentLight = new EnvironmentLight(4.0f, WHITE,
                                                            stratS);
  world.addLight(environmentLight);*/

  //Directional* directional = new Directional(WHITE, Vector(0,0,-1), 2 );

  //world.addLight(directional);

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new TrexNormScene(options);
}