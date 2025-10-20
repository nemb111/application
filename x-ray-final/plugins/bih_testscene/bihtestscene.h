#pragma once

#ifndef BIHTESTSCENE_H_
#define BIHTESTSCENE_H_

#include "scene.h"

class BIHTestscene : public Scene
{
public:
  BIHTestscene(const Options& options);
  virtual ~BIHTestscene(void);

  virtual void add(World& world);
};

#endif //BIHTESTSCENE_H_