
#pragma once

#ifndef NORENDERER_H_
#define NORENDERER_H_


#include "renderer.h"


class NoRenderer : public Renderer
{
public:


  // bricht den Rendervorgang sofort ab, indem false zurückgegeben wird.
  virtual bool Render(const SampleList& sampleList, uint iteration) const
  {
    return false;
  }
  virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f) const
  {
    return BLACK;
  }


};




#endif //NORENDERER_H_