#pragma once

#ifndef SCENELOADER_H_
#define SCENELOADER_H_


#include "x-ray.h"


// Forward Deklarationen
class Options;
class Scene;


class SceneLoader
{
  typedef Scene* (*CreateScene)(const Options& options);

public:
  SceneLoader(const Options& options);
  virtual ~SceneLoader(void);

  Scene* load(const Options& options);
  void unload();


private:
  stdstring _dllFile;
  HINSTANCE _hInstLibrary;
  Scene* _scene;
};



#endif //SCENELOADER_H_