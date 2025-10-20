#pragma once

#ifndef DEBUGSCENE_H_
#define DEBUGSCENE_H_

#include "scene.h"
class DebugScene : public Scene
{
public:
  DebugScene(const Options& options);
  virtual ~DebugScene(void);

  virtual void add(World& world);
  virtual bool init(World& world);
  virtual void destroyWorld();
};

#endif //DEBUGSCENE_H_