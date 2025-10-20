#include "adprenderer.h"

#include <limits>


//_____________________________________________________________________________
ADPrenderer::ADPrenderer(int maxDepth/*=1*/)
{
  _maxDepth = maxDepth;
}


//_____________________________________________________________________________
ADPrenderer::~ADPrenderer(void)
{
}


//_____________________________________________________________________________
Spectrum
ADPrenderer::Li(const Ray& ray, const int depth, float contribution/*=1.f*/)
{
  assert(_world->accelerator != NULL);
  assert(_world->shapes.size() > 0);
  assert(depth >= 0);
  assert(contribution >= 0 && contribution <= 1.f);


  double t;
  double tmin = std::numeric_limits<double>::max();
  ShadeRec shadeRec(*_world);
  ShadeRec resultRec(*_world);
  shadeRec.depth = depth;
  shadeRec.contribution = contribution;

  if (shadeRec.depth > _maxDepth || contribution < _cullCoeff)
    return BLACK;
  else
  {

    bool hit = false;
    for (size_t i=0; i<_world->shapes.size(); i++)
    {
      if (_world->shapes[i]->hit(ray, t, shadeRec) && (t < tmin) )
      {
        tmin = t;
        hit = true;
        shadeRec.ray = ray;
        shadeRec.t   = t;
        resultRec = shadeRec;
      }
      
    }
    if (hit)
      return resultRec.material->shade(resultRec);
    else
      return *(_world->backgroundColor);
  }
}


//_____________________________________________________________________________
Spectrum
ADPrenderer::globalLi(const Ray& ray, int depth, float contribution/*=1.f*/)
  const
{
  assert(_world->accelerator != NULL);
  assert(_world->shapes.size() > 0);
  assert(depth >= 0);
  assert(contribution >= 0 && contribution <= 1.f);


  //double t;
  //double tmin = std::numeric_limits<double>::max();
  //ShadeRec shadeRec(*_world);
  //ShadeRec resultRec(*_world);
  //shadeRec.depth = depth;
  //shadeRec.contribution = contribution;

  //if (shadeRec.depth > _maxDepth || contribution < _cullCoeff)
  //  return BLACK;
  //else
  //{

  //  bool hit = false;
  //  for (size_t i=0; i<_world->shapes.size(); i++)
  //  {
  //    if (_world->shapes[i]->hit(ray, t, shadeRec) && (t < tmin) )
  //    {
  //      tmin = t;
  //      hit = true;
  //      shadeRec.ray = ray;
  //      shadeRec.t   = t;
  //      resultRec = shadeRec;
  //    }
  //    
  //  }
  //  if (hit)
  //    return resultRec.material->shade(resultRec);
  //  else
  //    return *(_world->backgroundColor);
  //}

  return GREEN;
  IOUT("Nicht implementiert.");
}
