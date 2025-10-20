#pragma once

#ifndef SIMPLESCENE_H_
#define SIMPLESCENE_H_

#include "scene.h"

class SimpleScene : public Scene
{
public:
  SimpleScene(const Options& options)
   : Scene(options)
  {}
  ~SimpleScene(){}

  virtual void add(World& world);
 /* virtual bool init(World& world);
  virtual void destroyWorld();*/

};









#endif//SIMPLESCENE_H_