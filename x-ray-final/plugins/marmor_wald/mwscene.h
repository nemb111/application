#pragma once

#ifndef MWSCENE_H_
#define MWSCENE_H_


#include "scene.h"

class MWScene : public Scene
{
public:
  MWScene(const Options& options);
  virtual ~MWScene(void);

  virtual void add(World& world);
};


#endif //MWSCENE_H_