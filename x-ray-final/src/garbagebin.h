#pragma once

#ifndef GARBAGEBIN_H_
#define GARBAGEBIN_H_

#include <list>

#include "meshparser.h"
#include "normalmap.h"



//Forward Deklarationen
class Accelerator;
class Camera;
class Light;
class Material;
class MeshParser;
class NormalMap;
class Shape;
class Texture;
class UnitSampler;


class GarbageBin
{
friend class Parser;

public:
  GarbageBin(void);
  ~GarbageBin(void);

  template<class T>
  void mrkDel(T pointer);
  template<class T>
  void mrkDelA(T pointer);

private:
  void cleanUp();

  HANDLE _mrkDelMutex;
  HANDLE _mrkDelAMutex;

  std::list<Accelerator*> _accPointer;
  std::list<Accelerator*> _accPointerA;
  std::list<Camera*>      _camPointer;
  std::list<Camera*>      _camPointerA;
  std::list<Light*>       _lgtPointer;
  std::list<Light*>       _lgtPointerA;
  std::list<Material*>    _matPointer;
  std::list<Material*>    _matPointerA;
  std::list<MeshParser*>  _mepPointer;
  std::list<MeshParser*>  _mepPointerA;
  std::list<NormalMap*>   _nmpPointer;
  std::list<NormalMap*>   _nmpPointerA;
  std::list<Shape*>       _shpPointer;
  std::list<Shape*>       _shpPointerA;
  std::list<Texture*>     _texPointer;
  std::list<Texture*>     _texPointerA;
  std::list<UnitSampler*> _uspPointer;
  std::list<UnitSampler*> _uspPointerA;

};

#include "garbagebin.inl"

#endif //GARBAGEBIN_H_