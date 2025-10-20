#pragma once

#ifndef CBHSCENE_H_
#define CBHSCENE_H_


#include "scene.h"

class CBHScene : public Scene
{
public:
  CBHScene(const Options& options);
  virtual ~CBHScene(void);

  virtual void add(World& world);
};


#endif //CBHSCENE_H_