#include "stdafx.h"

//#include "accelerator.h"
//#include "camera.h"
//#include "light.h"
//#include "material.h"
//#include "meshparser.h"
//#include "shape.h"
//#include "texture.h"
//#include "unitsampler.h"
//#include "garbagebin.h"

//_____________________________________________________________________________
 GarbageBin::GarbageBin(void)
  : _mrkDelMutex(CreateMutex(NULL, FALSE, NULL) ), 
    _mrkDelAMutex(CreateMutex(NULL, FALSE, NULL) )
{
}


//_____________________________________________________________________________
 GarbageBin::~GarbageBin(void)
{
}


 //_____________________________________________________________________________
void
GarbageBin::cleanUp()
{
  for (std::list<Accelerator*>::iterator it = _accPointer.begin();
       it != _accPointer.end(); it++)
    delete *it;
  for (std::list<Accelerator*>::iterator it = _accPointerA.begin();
       it != _accPointerA.end(); it++)
    delete[] *it;

  for (std::list<Camera*>::iterator it = _camPointer.begin();
       it != _camPointer.end(); it++)
    delete *it;
  for (std::list<Camera*>::iterator it = _camPointerA.begin();
       it != _camPointerA.end(); it++)
    delete[] *it;

  for (std::list<Light*>::iterator it = _lgtPointer.begin();
       it != _lgtPointer.end(); it++)
    delete *it;
  for (std::list<Light*>::iterator it = _lgtPointerA.begin();
       it != _lgtPointerA.end(); it++)
    delete[] *it;

  for (std::list<Material*>::iterator it = _matPointer.begin();
       it != _matPointer.end(); it++)
    delete *it;
  for (std::list<Material*>::iterator it = _matPointerA.begin();
       it != _matPointerA.end(); it++)
    delete[] *it;

  for (std::list<MeshParser*>::iterator it = _mepPointer.begin();
       it != _mepPointer.end(); it++)
    delete *it;
  for (std::list<MeshParser*>::iterator it = _mepPointerA.begin();
       it != _mepPointerA.end(); it++)
    delete[] *it;

  for (std::list<NormalMap*>::iterator it = _nmpPointer.begin();
       it != _nmpPointer.end(); it++)
    delete *it;
  for (std::list<NormalMap*>::iterator it = _nmpPointerA.begin();
       it != _nmpPointerA.end(); it++)
    delete[] *it;

  for (std::list<Shape*>::iterator it = _shpPointer.begin();
       it != _shpPointer.end(); it++)
    delete *it;
  for (std::list<Shape*>::iterator it = _shpPointerA.begin();
       it != _shpPointerA.end(); it++)
    delete[] *it;

  for (std::list<Texture*>::iterator it = _texPointer.begin();
       it != _texPointer.end(); it++)
    delete *it;
  for (std::list<Texture*>::iterator it = _texPointerA.begin();
       it != _texPointerA.end(); it++)
    delete[] *it;

  for (std::list<UnitSampler*>::iterator it = _uspPointer.begin();
       it != _uspPointer.end(); it++)
    delete *it;
  for (std::list<UnitSampler*>::iterator it = _uspPointerA.begin();
       it != _uspPointerA.end(); it++)
    delete[] *it;


  _accPointer.clear();
  _accPointerA.clear();
  _camPointer.clear();
  _camPointerA.clear();
  _lgtPointer.clear();
  _lgtPointerA.clear();
  _matPointer.clear();
  _matPointerA.clear();
  _mepPointer.clear();
  _mepPointerA.clear();
  _nmpPointer.clear();
  _nmpPointerA.clear();
  _shpPointer.clear();
  _shpPointerA.clear();
  _texPointer.clear();
  _texPointerA.clear();
  _uspPointer.clear();
  _uspPointerA.clear();
}