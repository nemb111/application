#pragma once

#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include "renderer.h"

typedef std::vector<std::pair<float, Sample> > PrioSampleList;


class PathTracer : public Renderer
{
public:
  PathTracer(int maxDepth=1);
  virtual ~PathTracer(void);

  virtual bool Render(const SampleList& sampleList, uint iteration) const;
  virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f) const;

protected:
  int _maxDepth;

};


#endif //PATHTRACER_H_