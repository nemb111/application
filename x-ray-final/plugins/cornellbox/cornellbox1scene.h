#pragma once

#ifndef CORNELLBOX1SCENE_H_
#define CORNELLBOX1SCENE_H_


#include "scene.h"

class CornellBox1Scene : public Scene
{
public:
  CornellBox1Scene(const Options& options);
  virtual ~CornellBox1Scene(void);

  virtual void add(World& world);
};


#endif //CORNELLBOX1SCENE_H_