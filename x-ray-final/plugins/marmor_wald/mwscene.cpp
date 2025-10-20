
#include "stdafx.h"

//#include <ctime>

#include "mwscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
MWScene::MWScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
MWScene::~MWScene(void)
{
}


//_____________________________________________________________________________
void
MWScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


   Spectrum outFilter, inFilter;
  outFilter[0] = outFilter[1] = outFilter[2] = 1.f;
  inFilter[0] = inFilter[1] = inFilter[2] = 1.0f;
  Dielectric* dielectric = 
    new Dielectric(WHITE, WHITE, 0.01, 0.1, .60, 500, 1.0f, 1.15f, inFilter,
                   outFilter, 1.f, 0.0009f);
 
  
  // Gehe alle Shapes durch, suche nach horse und weise rmirror zu
  for (size_t i=0; i<world.shapes.size(); i++)
  {
    if (world.shapes[i]->getName() == stdstring(_T("horse") ) )
    {
      if (dynamic_cast<Instance*>(world.shapes[i]) != nullptr)
        const_cast<Shape*>(dynamic_cast<Instance*>(world.shapes[i])
          ->getContent()
        )->setMaterial(dielectric);
    }
  }

}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new MWScene(options);
}