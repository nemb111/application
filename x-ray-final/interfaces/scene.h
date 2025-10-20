#pragma once

#ifndef SCENE_H_
#define SCENE_H_

#include "options.h"

#if defined DLL_EXPORT || defined _WINDLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif


//Forward Deklarationen
class World;


class Scene
{
public:
  Scene(const Options& options)
  {
    // Die DLL wird zur Laufzeit geladen und kennt daher nicht die bis dahin
    // erzeugten statischen Variablen. Diese werden deshalb per Konstruktur
    // übergeben und erneut gesetzt.
    Options::getInstance() = options;
  }
  virtual ~Scene(){}

  virtual void add(World& world) = 0;
  /*virtual bool init(World& world) = 0;
  virtual void destroyWorld() = 0;*/

protected:

};

extern "C"
{
  DECLDIR Scene* createScene(Options& options);
}


#endif//SCENE_H_