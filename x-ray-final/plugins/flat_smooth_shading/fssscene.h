#pragma once

#ifndef FSSSCENE_H_
#define FSSSCENE_H_


#include "scene.h"

class FSSScene : public Scene
{
public:
  FSSScene(const Options& options);
  virtual ~FSSScene(void);

  virtual void add(World& world);
};


#endif //CBHSCENE_H_