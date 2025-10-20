#pragma once

#ifndef TRANSPARENCYSCENE_H_
#define TRANSPARENCYSCENE_H_

#define DLL_EXPORT

#include "scene.h"


class TransparencyScene : public Scene
{
public:
  TransparencyScene(const Options& options);
  ~TransparencyScene();

  virtual void add(World& world);
};



#endif //TRANSPARENCYSCENE_H_