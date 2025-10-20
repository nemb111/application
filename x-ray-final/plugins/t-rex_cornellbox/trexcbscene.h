#pragma once

#ifndef TREXCBSCENE_H_
#define TREXCBSCENE_H_


#include "scene.h"

class TrexCBScene : public Scene
{
public:
  TrexCBScene(const Options& options);
  virtual ~TrexCBScene(void);

  virtual void add(World& world);
};


#endif //TREXCBSCENE_H_