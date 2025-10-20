#pragma once

#ifndef CBSSCENE_H_
#define CBSSCENE_H_


#include "scene.h"

class CBSScene : public Scene
{
public:
  CBSScene(const Options& options);
  virtual ~CBSScene(void);

  virtual void add(World& world);
};


#endif //CBSSCENE_H_