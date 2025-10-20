#pragma once

#ifndef BIHBENCHMARKSCENE_H_
#define BIHBENCHMARKSCENE_H_

#include "scene.h"

class BIHBenchmarkScene : public Scene
{
public:
  BIHBenchmarkScene(const Options& options);
  virtual ~BIHBenchmarkScene(void);

  virtual void add(World& world);
};

#endif //BIHBENCHMARKSCENE_H_