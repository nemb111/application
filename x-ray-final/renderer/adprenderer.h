#pragma once

#ifndef ADPRENDERER_H_
#define ADPRENDERER_H_

#include "renderer.h"

//Ein Renderer der Ambient-, Directional und Pointlights benutzt
class ADPrenderer : public Renderer
{
public:
  ADPrenderer(int maxDepth=1);
  virtual ~ADPrenderer(void);

  virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f);
  virtual Spectrum globalLi(const Ray& ray, int depth,
                            float contribution=1.f) const;
};

#endif//ADPRENDERER_H_