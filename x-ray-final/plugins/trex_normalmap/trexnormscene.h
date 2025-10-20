#pragma once

#ifndef TREXNORMSCENE_H_
#define TREXNORMSCENE_H_

#include "scene.h"

class TrexNormScene : public Scene
{
public:
  TrexNormScene(const Options& options);
  virtual ~TrexNormScene(void);

  virtual void add(World& world);
};

#endif //TREXNORMSCENE_H_