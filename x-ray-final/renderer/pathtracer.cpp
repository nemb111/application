#include "stdafx.h"

//#include "pathtracer.h"

//_____________________________________________________________________________
PathTracer::PathTracer(int maxDepth/*=1*/)
  : _maxDepth(maxDepth)
{
}


//_____________________________________________________________________________
PathTracer::~PathTracer(void)
{
}


//_____________________________________________________________________________
bool
PathTracer::Render(const SampleList& sampleList, uint iteration) const
{
  float energyDiff = 0;
  Spectrum resSpec(BLACK);


  for (size_t i=0; i<sampleList.size(); i++)
  {
    // Bedingung die Überprüft, ob nur ein einziger Pixel gerendert werden soll
    if (_debugPixelSet && !(sampleList[i].pxlIdxX == _debugPixelX &&
        sampleList[i].pxlIdxY == _debugPixelY) )
    {
      continue;
    }

    resSpec = BLACK;
    for (size_t j=0; j<sampleList[i].rays.size(); j++)
    {
      resSpec += Li(sampleList[i].rays[j], 0, 1.0f) * 
                    sampleList[i].weigths[j];

    }
    // Prüfe ob der neu berechnete Wert mehr Farbinformation bringt als der
    // alte. Falls dem so ist ersetze diesen.
    energyDiff = resSpec.magnitude() - 
                 _world->film->getGlobIllPixelSpectrum(
                    sampleList[i].pxlIdxX, 
                    sampleList[i].pxlIdxY).magnitude();


    if (energyDiff > 0)
    {
      writeBackGlobIllSample(sampleList[i].pxlIdxX, 
                             sampleList[i].pxlIdxY, resSpec);
    }
  }

  return true;
}


//_____________________________________________________________________________
Spectrum 
PathTracer::Li(const Ray& ray, const int depth,
               float contribution) const
{
  assert(_world->accelerator != NULL);
  assert(_world->shapes.size() > 0);
  assert(depth >= 0);
  assert(contribution >= 0 && contribution <= 1.f);

  double tmin = std::numeric_limits<double>::max();
  ShadeRec shadeRec(*_world);
  shadeRec.depth = depth;
  shadeRec.contribution = contribution;

  if (shadeRec.depth > _maxDepth || contribution < _cullCoeff)
    return _world->renderer->Li(ray, 0, 1.0f);
  else
  {
    bool hit =_world->accelerator->hit(ray, tmin, shadeRec);

    if (hit)
    {
      shadeRec.ray = ray;
      shadeRec.t   = tmin;
      return shadeRec.material->globalShade(shadeRec);
    }
    else
    {
      shadeRec.t = std::numeric_limits<double>::max();
      return *(_world->backgroundColor);
    }
  }
}