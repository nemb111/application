
#include "stdafx.h"

//#include <ctime>

#include "cbhscene.h"
//#include "x-ray_includes.h"


//_____________________________________________________________________________
CBHScene::CBHScene(const Options& options)
  : Scene(options)
{
}


//_____________________________________________________________________________
CBHScene::~CBHScene(void)
{
}


//_____________________________________________________________________________
void
CBHScene::add(World& world)
{
  time_t t = time(NULL);
  Options::getInstance().outfile = _T("out/") + std::to_wstring(time(NULL) ) + _T(".png");


  Spectrum outFilter, inFilter;
  outFilter[0] = outFilter[1] = outFilter[2] = 1.f;
  inFilter[0] = inFilter[1] = inFilter[2] = 1.0f;
  Dielectric* dielectric = 
    new Dielectric(WHITE, WHITE, 0.01, 0.1, .60, 500, 1.0f, 1.35f, inFilter,
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


  UnitSampler* uSampler = /*new MidpointS;*/ new StratifiedS(100);
  Emissive* emissive = new Emissive;
  LightShape* lightShape = new Rect(Point(213, 548.8, -332), Vector(130,0,0),
                                    Vector(0,0,105), Normal(0,-1,0) );
  AreaLight* areaLight = new AreaLight(6000000000, 30000000, WHITE, lightShape,
                                       emissive, uSampler, world);

  world.addLight(areaLight);
}


//_____________________________________________________________________________
Scene*
createScene(Options& options)
{
  return new CBHScene(options);
}