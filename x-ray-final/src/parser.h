#pragma once

#ifndef PARSER_H_
#define PARSER_H_

#include <Windows.h>

#include <functional>
#include <queue>
#include <string>

#include "garbagebin.h"
#include "mesh.h"
#include "sceneloader.h"
#include "settingloader.h"

//Parser(Assimp)
#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/scene.h" // Output data structure
#include "assimp/postprocess.h" // Post processing flags


//Image Library (DevIl)
#include "IL\il.h"


//Forward Deklarationen
class Scene;
class Settings;
class World;

#define GBin Parser::GB



struct threadStruct
{
  const World* world;
  int atomicCntr;
  uint nSamples;
  size_t sampleListQueueSize;
  std::queue<SampleList>* sampleListQueue;
};

class Parser
{
public:
  static bool directIllumination(void* args);
  static bool globalIllumination(void* args);
  static GarbageBin GB;
  

  Parser();
  virtual ~Parser();

  bool createSettings();
  void destroySettings();
  bool createWorld(const stdstring& sceneFile, GUIcomm* guiComm);
  void destroyWorld();
  void startRendering();

protected:

  void applySettings();
  void fillMesh(Mesh& mesh, const stdstring& sceneFile, const aiScene* sc,
                const aiMesh* aimesh);
  Camera*   createCamera(const stdstring& sceneFile, const aiScene* scene);
  Light*    createLights(const stdstring& sceneFile, const aiScene* scene,
                         uint i);
  Material* createMaterial(const stdstring& sceneFile, const aiScene* sc,
                           const aiMesh* aimesh);
  Texture*  createTexture(const stdstring& sceneFile, const aiScene* sc,
                          const aiMesh* aimesh, aiTextureType texType);
  void parseSceneFromFile(const stdstring& sceneFile);
  void preProcessing();
  void postProcessing();
  void parseRecursive(const stdstring& sceneFile, const aiScene* sc,
                      const aiNode* nd);


  Scene* _scene;
  Settings* _settings;
  SceneLoader* _sceneLoader;
  SettingLoader* _settingLoader;
  World* _world;

private:



  static HANDLE _mutex;

};


#endif//PARSER_H_

