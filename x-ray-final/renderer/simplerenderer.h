#pragma once

#ifndef SIMPLERENDER_H_
#define SIMPLERENDER_H_

#include "renderer.h"
#include "spectrum.h"


class SimpleRenderer : public Renderer
{
public:
  SimpleRenderer(void);
  virtual ~SimpleRenderer(void);

  //virtual void Render(World* world, const SampleList& sampleList);
  virtual Spectrum Li(const Ray& ray);
  virtual Spectrum globalLi(const Ray& ray, int depth,
                            float contribution=1.f) const;

};


#endif//SIMPLERENDERER_H_