#pragma once

#ifndef DEFAULTSCENE_H_
#define DEFAULTSCENE_H_

#include "scene.h"

class DefaultScene : public Scene
{
public:
  DefaultScene(const Options& options);
  virtual ~DefaultScene(void);

  virtual void add(World& world);
};

#endif //DEFAULTSCENE_H_