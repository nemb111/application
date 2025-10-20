#pragma once

#ifndef TREXSCENE_H_
#define TREXSCENE_H_

#include "scene.h"

class TrexScene : public Scene
{
public:
  TrexScene(const Options& options);
  virtual ~TrexScene(void);

  virtual void add(World& world);
};

#endif //TREXSCENE_H_