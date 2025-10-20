#include "stdafx.h"

//#include "accelrenderer.h"


//_____________________________________________________________________________
AccelRenderer::AccelRenderer(int maxDepth, int maxGlobIlDepth/*=1*/)
{
  _maxDepth = maxDepth;
  //_maxGlobIlDepth = maxGlobIlDepth;
}


//_____________________________________________________________________________
AccelRenderer::~AccelRenderer(void)
{
}


//_____________________________________________________________________________
Spectrum
AccelRenderer::Li(const Ray& ray, const int depth, float contribution/*=1.f*/)
const
{
  assert(_world->accelerator != NULL);
  assert(_world->shapes.size() > 0);
  assert(depth >= 0);
  assert(contribution >= 0);

  double tmin = std::numeric_limits<double>::max();
  ShadeRec shadeRec(*_world);
  shadeRec.depth = depth;
  shadeRec.contribution = contribution;

  if (shadeRec.depth > _maxDepth || contribution < _cullCoeff)
    return BLACK;
  else
  {
    bool hit =_world->accelerator->hit(ray, tmin, shadeRec);

    if (hit)
    {
      shadeRec.ray = ray;
      shadeRec.t   = tmin;
      return shadeRec.material->areaLightShade(shadeRec);
    }
    else
    {
      shadeRec.t = std::numeric_limits<double>::max();
      return *(_world->backgroundColor);
    }
  }
}


//_____________________________________________________________________________
//Spectrum
//AccelRenderer::globalLi(const Ray& ray, int depth, float contribution/*=1.f*/)
//const
//{
//  assert(_world->accelerator != NULL);
//  assert(_world->shapes.size() > 0);
//  assert(depth >= 0);
//  assert(contribution >= 0 && contribution <= 1.f);
//
//  double tmin = std::numeric_limits<double>::max();
//  ShadeRec shadeRec(*_world);
//  shadeRec.depth = depth;
//  shadeRec.contribution = contribution;
//
//  if (shadeRec.depth > _maxGlobIlDepth || contribution < _cullCoeff)
//    return BLACK;/*Li(ray, 0, 1.0f);*/
//  else
//  {
//    bool hit =_world->accelerator->hit(ray, tmin, shadeRec);
//
//    if (hit)
//    {
//      shadeRec.ray = ray;
//      shadeRec.t   = tmin;
//      return shadeRec.material->globalShade(shadeRec);
//    }
//    else
//    {
//      shadeRec.t = std::numeric_limits<double>::max();
//      return *(_world->backgroundColor);
//    }
//  }
//}