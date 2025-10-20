#include "simplerenderer.h"

#include "instance.h"

//_____________________________________________________________________________
SimpleRenderer::SimpleRenderer(void)
{
}


//_____________________________________________________________________________
SimpleRenderer::~SimpleRenderer(void)
{
}


//_____________________________________________________________________________
Spectrum
SimpleRenderer::Li(const Ray& ray)
{
  double t;
  ShadeRec shadeRec(*_world);
  for (size_t i=0; i<_world->shapes.size(); i++)
  {
    if (_world->shapes[i]->hit(ray, t, shadeRec) )
      return RED;
  }
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
SimpleRenderer::globalLi(const Ray& ray, int depth,
                            float contribution/*=1.f*/) const
{
  return GREEN;
  IOUT("Nicht implementiert.");
}