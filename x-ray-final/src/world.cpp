#include "stdafx.h"

//#include "world.h"
//
//#include "ambient.h"
//#include "norenderer.h"


//_____________________________________________________________________________
//Threadlocal* World::threadlocal;


//_____________________________________________________________________________
World::World(void)
 : film(NULL), camera(NULL), cameraDirIllSampler(NULL),
   cameraGlobIllSampler(NULL),
   accelerator(NULL),
   backgroundColor(BLACK), renderer(NULL), globalRenderer(new NoRenderer),
   ambientLight(new Ambient() ),
   filter(NULL), guiComm(NULL)
{
}


//_____________________________________________________________________________
World::~World(void)
{
  delete ambientLight;
  ambientLight = 0;
}
