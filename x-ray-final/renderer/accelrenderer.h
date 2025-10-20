#pragma once

#ifndef ACCELRENDERER_H_
#define ACCELRENDERER_H_

#include "renderer.h"

class AccelRenderer : public Renderer
{
public:
  AccelRenderer(int maxDepth, int maxGlobIlDepth=1);
  virtual ~AccelRenderer(void);

  virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f) const;
 /* virtual Spectrum globalLi(const Ray& ray, int depth,
                            float contribution=1.f) const;*/
};


#endif //ACCELRENDERER_H_