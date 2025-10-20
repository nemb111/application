#pragma once

#ifndef SSPSCENE_H_
#define SSPSCENE_H_


#include "scene.h"

class SSPScene : public Scene
{
public:
  SSPScene(const Options& options);
  virtual ~SSPScene(void);

  virtual void add(World& world);
};


#endif //SSPSCENE_H_