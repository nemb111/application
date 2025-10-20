#pragma once

#ifndef ANTIALIASINGSCENE_H_
#define ANTIALIASINGSCENE_H_

#include "scene.h"

class AntialiasingScene : public Scene
{
public:
  AntialiasingScene(const Options& options);
  virtual ~AntialiasingScene(void);

  virtual void add(World& world);
};

#endif //ANTIALIASINGSCENE_H_