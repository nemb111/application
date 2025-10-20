#include "stdafx.h"

#include "stopwatch.h"


//#include <Windows.h>
//#include <algorithm>
//#include <fstream>
//#include <functional>
//#include <memory>
//#include <queue>
//#include <vector>


//#include "arealight.h"
//#include "assimpmeshtriangle.h"
//#include "directional.h"
//#include "disc.h"
//#include "imageio.h"
//#include "imagetex.h"
//#include "instance.h"
//#include "normalmap.h"
//#include "assimpmaterial.h"
//#include "options.h"
//#include "parser.h"
//#include "pinhole.h"
//#include "pointlight.h"
//#include "renderer.h"
//#include "scene.h"
//#include "settings.h"
//#include "stratifieds.h"
//#include "x-ray.h"



//_____________________________________________________________________________
HANDLE Parser::_mutex = CreateMutex(NULL, FALSE, NULL);


//_____________________________________________________________________________
GarbageBin Parser::GB;


//_____________________________________________________________________________
bool
Parser::directIllumination(void* args)
{
  int mutex_result = 0;
  threadStruct* tStruct = (threadStruct*)args;
  SampleList sampleList;


  // Jeder Thread erzeugt seinen eigenen lokalen Speicherbereich
  //std::unique_ptr<Threadlocal> storage(new Threadlocal);
  

  while (!tStruct->world->guiComm->saveAndCancel() )
  {
    mutex_result = WaitForSingleObject(_mutex,      //handle to mutex
                                       INFINITE);   //no time-out interval
    switch (mutex_result) 
    {
      // The thread got ownership of the mutex
      case WAIT_OBJECT_0:
        if (!tStruct->sampleListQueue->empty() )
        {
          sampleList = tStruct->sampleListQueue->front();
          tStruct->sampleListQueue->pop();
          ReleaseMutex(_mutex);
        }
        else
        {
          ReleaseMutex(_mutex);
          return TRUE;
        }
        break; 

      // The thread got ownership of an abandoned mutex
      // The database is in an indeterminate state
      case WAIT_ABANDONED: 
        return FALSE; 
    }

    //Starte hier jetzt den Renderer zusammen mit der Welt und dem Subsampler
    tStruct->world->renderer->Render(sampleList, 0);

  }

  return FALSE;
}


//_____________________________________________________________________________
bool
Parser::globalIllumination(void* args)
{
  bool proceed = true;
  int mutex_result = 0;
  threadStruct* tStruct = (threadStruct*)args;
  uint nSamples = tStruct->nSamples;
  size_t sampleListQueueSize = tStruct->sampleListQueueSize;
  uint iteration = 0;
  SampleList sampleList;


  while (proceed && !tStruct->world->guiComm->saveAndCancel() )
  {
    mutex_result = WaitForSingleObject(_mutex,      //handle to mutex
                                       INFINITE);   //no time-out interval
    switch (mutex_result) 
    {
      // The thread got ownership of the mutex
      case WAIT_OBJECT_0:
        if (!tStruct->sampleListQueue->empty() )
        {
          iteration = 
            static_cast<uint>(tStruct->atomicCntr / sampleListQueueSize) + 1;
          tStruct->atomicCntr++;
          sampleList = tStruct->sampleListQueue->front();
          tStruct->sampleListQueue->pop();
          ReleaseMutex(_mutex);
        }
        else
        {
          ReleaseMutex(_mutex);
          continue;
        }
        break;

      // The thread got ownership of an abandoned mutex
      // The database is in an indeterminate state
      case WAIT_ABANDONED: 
        return FALSE; 
    }

    //Starte hier jetzt den Renderer zusammen mit der Welt und dem Subsampler
    proceed = tStruct->world->globalRenderer->Render(sampleList, iteration);

    // Füge die eben abgearbeitete Liste von Sample wieder hinten in die
    // queue ein
    mutex_result = WaitForSingleObject(_mutex,      //handle to mutex
                                       INFINITE);   //no time-out interval
    switch (mutex_result) 
    {
      // The thread got ownership of the mutex
      case WAIT_OBJECT_0:
        tStruct->sampleListQueue->push(sampleList);
        ReleaseMutex(_mutex);
        break;

      // The thread got ownership of an abandoned mutex
      // The database is in an indeterminate state
      case WAIT_ABANDONED: 
        return FALSE; 
    }

  }

  return TRUE;
}


//_____________________________________________________________________________
Parser::Parser()
{
}


//_____________________________________________________________________________
Parser::~Parser()
{
  delete _world;
  _world = 0;

  GB.cleanUp();
}


//_____________________________________________________________________________
bool
Parser::createSettings()
{
  IOUT("Erzeuge Settings.");
  Options& options = Options::getInstance();

  _settingLoader = new SettingLoader(options);
  _settings = _settingLoader->load(options);


  return true;
}


//_____________________________________________________________________________
void
Parser::destroySettings()
{
  _settingLoader->unload();

}


//_____________________________________________________________________________
bool
Parser::createWorld(const stdstring& sceneFile, GUIcomm* guiComm)
{
  Options& options = Options::getInstance();

  IOUT("Erzeuge World.");
  _world = new World;

  IOUT("Wende Settings an.");
  applySettings();

  _sceneLoader = new SceneLoader(options);
  _scene = _sceneLoader->load(options);
  
  // Liest Meshes und Texturen aus einer Datei und fügt diese der Welt hinzu
  parseSceneFromFile(sceneFile);
  // Wird verwendet um der Szene "händisch" objekte, Kamera und Licht 
  // hinzuzufügen
  if (_scene)
    _scene->add(*_world);
  _world->guiComm = guiComm;

  //Globale Beschleunigungsdatenstruktur erstellen
  IOUT("Erzeuge globale Beschleunigungsstruktur...")
  _world->accelerator = _settings->_globalAccelerator;
  _world->accelerator->addShapes(_world->shapes);
  _world->accelerator->create();
  IOUT("...fertig.")

  //Stelle sicher, dass eine Kamera erzeugt wurde
  if (_world->camera == NULL)
  {
    EOUT("Es wurde keine Kamera erzeugt, das Rendern kann nicht gestartet"
         " werden.");
    std::cin.get();
    exit(1);
  }

  return true;
}


//_____________________________________________________________________________
void
Parser::destroyWorld()
{
  delete _world;
  _world = 0;
  
  _sceneLoader->unload();
  
  GB.cleanUp(); 
}


//_____________________________________________________________________________
void
Parser::startRendering()
{ 
  /*++++++++++++++++++++++++++Preprocessing+++++++++++++++++++++++++++++++++*/
  IOUT("Starte Preprocessing");
  
  const Options& options = Options::getInstance();
  uint nPixels = _world->film->getPxlWidth() * _world->film->getPxlHeight();
  uint nCores = numSystemCores(Options::getInstance() );
  uint nTasks = std::max(32 * nCores, nPixels / (16*16));
       nTasks = RoundUpPow2(nTasks);
  threadStruct dirIllStruct;
  threadStruct globIllStruct;
  dirIllStruct.world =  _world;
  globIllStruct.world = _world;
  dirIllStruct.atomicCntr = 0;
  globIllStruct.atomicCntr = 0;
  
  HANDLE* threads = new HANDLE[nCores];
  DWORD   threadID;
  if (_mutex == NULL)
  {
    EOUT("Erzeugen des Mutex ist fehlgeschlagen");
    std::cin.get();
    exit(1);
  }

  uint nSamples = nPixels / nTasks;
  nSamples = (nSamples == 0) ? 1 : nSamples;
  std::queue<SampleList> dirIllQueue;
  std::queue<SampleList> globIllQueue;

  SampleList dirIllSampleList;
  SampleList globIllSampleList;
  dirIllSampleList.reserve(nSamples);
  globIllSampleList.reserve(nSamples);
  while (_world->camera->getNextNSamples(nSamples, dirIllSampleList, 
                                         globIllSampleList) )
  {
    if (options.dirIll == true)
      dirIllQueue.push(dirIllSampleList); 
    if (options.globalIll == true)
    {
      globIllQueue.push(globIllSampleList);
    }

    dirIllSampleList.clear();
    globIllSampleList.clear();
  }

  dirIllStruct.nSamples = nSamples;
  globIllStruct.nSamples = nSamples;
  dirIllStruct.sampleListQueueSize = dirIllQueue.size();
  globIllStruct.sampleListQueueSize = globIllQueue.size();
  dirIllStruct.sampleListQueue = &dirIllQueue;
  globIllStruct.sampleListQueue = &globIllQueue;


  preProcessing();
  // Beide Renderer müssen initialisiert werden, da evt. der jeweils andere 
  // Renderer darauf zugreifen möchte
  IOUT("Initialisiere Renderer fuer direkte Beleuchtung.");
  _world->renderer->init(_world);
  IOUT("Initialisiere Renderer fuer indirekte Beleuchtung.");
  _world->globalRenderer->init(_world);
 

  
  /*--------------------------Preprocessing----------------------------------*/
  

  
  /*+++++++++++++++++++++++++++Parallelisieren++++++++++++++++++++++++++++++++*/

  IOUT("Starte Rendering auf " << nCores << " Thread(s)!");
  
  if (options.dirIll == true)
  {
    IOUT("Starte direkte Beleuchtung.");
    Stopwatch::start(_T("DirIll") );

    for (uint i=0; i<nCores; i++)
    {
      threads[i] = CreateThread(
                                NULL,       // default security attributes
                                0,          // default stack size
                                (LPTHREAD_START_ROUTINE) directIllumination, 
                                (void*)&dirIllStruct, // thread function arguments
                                0,          // default creation flags
                                &threadID); // receive thread identifier

      if( threads[i] == NULL )
      {
        EOUT("Thread " << i << "konnte nicht erzeugt werden");
        std::cin.get();
        exit(1);
      }
    }

  
    // Wait for all threads to terminate
    WaitForMultipleObjects(nCores, threads, TRUE, INFINITE);
    IOUT(_T("Direkte Beleuchtung abgeschlossen. Dauer: ") << 
         Stopwatch::end(_T("DirIll") )/1000. <<
         "s");
  }

  if (options.globalIll == true)
  {
    IOUT("Starte globale Beleuchtung.");
    Stopwatch::start(_T("globIll") );
    
    for (uint i=0; i<nCores; i++)
    {
      threads[i] = CreateThread(
                                NULL,       // default security attributes
                                0,          // default stack size
                                (LPTHREAD_START_ROUTINE) globalIllumination, 
                                (void*)&globIllStruct, // thread function arguments
                                0,          // default creation flags
                                &threadID); // receive thread identifier

      if( threads[i] == NULL )
      {
        EOUT("Thread " << i << "konnte nicht erzeugt werden");
        std::cin.get();
        exit(1);
      }
    }  

    // Wait for all threads to terminate
    WaitForMultipleObjects(nCores, threads, TRUE, INFINITE);
    Stopwatch::end(_T("globIll") );
  }

  // Close thread and mutex handles
  for (uint i=0; i < nCores; i++ )
    CloseHandle(threads[i]);

  CloseHandle(_mutex);
  /*------------------------------Parallisieren------------------------------*/


  /*++++++++++++++++++++++++++Postprocessing+++++++++++++++++++++++++++++++++*/
  IOUT("Starte PostProcessing.");
  postProcessing();
  /*--------------------------Postprocessing----------------------------------*/


  _world->film->writeImage();

  delete[] threads;
  threads = 0;


}



//_____________________________________________________________________________
void
Parser::preProcessing()
{
}


//_____________________________________________________________________________
void
Parser::postProcessing()
{
  if (_world->filter)
  {
    IOUT("Postprocessing: Starte Bildkorrektur.");
    Stopwatch::start(_T("Postprocessing") );
    _world->film->filter(_world->filter, _world->guiComm);
    IOUT("Das Postprocessing dauerte: " << Stopwatch::end(_T("Postprocessing") )
         << "ms.");
  }
}


//_____________________________________________________________________________
void
Parser::applySettings()
{
  Options& options = Options::getInstance();


  if (!_settings->_renderer)
  {
    EOUT("Es wurde kein Renderer für direkte Beleuchtung erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->renderer = _settings->_renderer;

  if (!_settings->_globalRenderer)
  {
    WOUT("Es wurde kein Renderer fuer globale Beleuchtung erzeugt. "
         "Globale Beleuchtung ist deaktiviert.");
    options.globalIll = false;
  }
  else
  {
    // Dran denken, dass bereits ein NoRenderer mit new erzeugt wurde. Deshalb
    // jetzt löschen
    delete _world->globalRenderer;
    _world->globalRenderer = _settings->_globalRenderer;

  }

 
  // Checke hier ob ein einzelner Pixel zu Debugging-Zwecken gerendert werden
  // soll. Falls ja, teile dies dem Renderer mit.
  if (options.debugPixelX < std::numeric_limits<unsigned int>::max() &&
      options.debugPixelY < std::numeric_limits<unsigned int>::max() )
  {
    _world->renderer->setDebugPixel(options.debugPixelX, options.debugPixelY);
    _world->globalRenderer->setDebugPixel(options.debugPixelX, options.debugPixelY);
  }


  if (!_settings->_film)
  {
    EOUT("Es wurde kein Film erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->film = _settings->_film;

  if (!_settings->_cameraDirIllSampler)
  {
    EOUT("Es wurde kein Sampler für die ViewPlane erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->cameraDirIllSampler = _settings->_cameraDirIllSampler;

  _world->cameraGlobIllSampler = _settings->_cameraGlobIllSampler;

  if (!_settings->_backgroundColor)
  {
    EOUT("Es wurde keine BackGroundColor erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->backgroundColor = _settings->_backgroundColor;

  if (!_settings->_globalAccelerator)
  {
    EOUT("Es wurde kein GlobalAccelerator erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->accelerator = _settings->_globalAccelerator;

  if (!_settings->_filter)
  {
    EOUT("Es wurde kein Filter erzeugt.");
    std::cin.get();
    exit(1);
  }
  else
    _world->filter = _settings->_filter;

}


//_____________________________________________________________________________
void
Parser::fillMesh(Mesh& mesh, const stdstring& sceneFile, const aiScene* sc,
                 const aiMesh* aimesh)
{

  if ((aimesh->mPrimitiveTypes - aiPrimitiveType_TRIANGLE) > 0)
  {
    EOUT("Es werden nur triangulierte Objekte unterstützt.");
    std::cin.get();
    exit(1);
  }

  // Vertices ins Mesh einfügen
  mesh.numVertices = aimesh->mNumVertices;
  mesh.vertices.clear();
  mesh.vertices.reserve(aimesh->mNumVertices);
  for (uint i=0; i<aimesh->mNumVertices; i++)
  {
    std::vector<double> vertice;
    vertice.push_back(aimesh->mVertices[i].x);
    vertice.push_back(aimesh->mVertices[i].y);
    vertice.push_back(aimesh->mVertices[i].z);
    
    mesh.vertices.push_back(vertice);
  }

  // Faces ins Mesh einfügen
  mesh.numTriangles = aimesh->mNumFaces;
  mesh.faces.clear();
  mesh.faces.reserve(aimesh->mNumFaces);
  for (uint i=0; i<aimesh->mNumFaces; i++)
  {
    std::vector<size_t> indices;
    for (uint j=0; j<aimesh->mFaces[i].mNumIndices; j++)
      indices.push_back(static_cast<size_t>(aimesh->mFaces[i].mIndices[j]) );

    mesh.faces.push_back(indices);
  }

  // Normalen ins Mesh einfügen
  if (aimesh->mNormals == NULL)
  {
    EOUT("Das Objekt besitzt keine Normalen.");
    std::cin.get();
    exit(1);
  }
  else
  {
    mesh.normals.clear();
    mesh.normals.reserve(aimesh->mNumVertices);
    for (uint i=0; i<aimesh->mNumVertices; i++)
    {
      Normal normal(aimesh->mNormals[i].x, aimesh->mNormals[i].y,
                    aimesh->mNormals[i].z);

      mesh.normals.push_back(normal);
    }
  }

  // UV-Koordinaten ins Mesh einfügen. Der Punkt 0x|0y befindet sich in der
  // unteren linken Ecke.
  if (aimesh->HasTextureCoords(0) )
  {
    mesh.u.clear();
    mesh.v.clear();
    for (uint i=0; i<aimesh->mNumVertices; i++)
    {
      //Normalisiere die UV Koordinaten auf [0, 1]
      float u = aimesh->mTextureCoords[0][i].x;
      float v = aimesh->mTextureCoords[0][i].y;
      if (u > 1.f || u < 0.f)
        u -= floor(u);
      if (v > 1.f || v < 0.f)
        v -= floor(v);


      mesh.u.push_back(u);
      mesh.v.push_back(v);
    }
  }

}


//_____________________________________________________________________________
Camera*
Parser::createCamera(const stdstring& sceneFile, const aiScene* scene)
{
  if (scene->HasCameras() )
  {
    aiCamera* aicamera = scene->mCameras[0];
    Camera* camera= new Pinhole(Point(aicamera->mPosition.x,
                                      aicamera->mPosition.y,
                                      aicamera->mPosition.z),
                                Normal(aicamera->mLookAt.x,
                                       aicamera->mLookAt.y,
                                       aicamera->mLookAt.z),
                                Normal(aicamera->mUp.x,
                                       aicamera->mUp.y,
                                       aicamera->mUp.z),
                                _world->film->getPxlWidth() *
                                _world->film->getPxlSize(),
                                aicamera->mHorizontalFOV);
    camera->setFilm(_world->film);
    camera->setUnitSampler(_world->cameraDirIllSampler,
                           _world->cameraGlobIllSampler);

    GBin.mrkDel<Camera*>(camera);

    return camera;
  }

  return NULL;
}


//_____________________________________________________________________________
Light*
Parser::createLights(const stdstring& sceneFile, const aiScene* scene,
                     uint i)
{
  aiLight* ailight = scene->mLights[i];
  Light*   light;

  Spectrum spec;
  float intensity;
  bool castsShadow;

  spec[0] = ailight->mColorDiffuse.b;
  spec[1] = ailight->mColorDiffuse.g;
  spec[2] = ailight->mColorDiffuse.r;

  switch (ailight->mType)
  {
    // Directional Lights erzeugen
    case aiLightSource_DIRECTIONAL:
    {
      uint numDirLights = -1;

      for (uint j=0; j<=i; j++)
        if (scene->mLights[j]->mType == aiLightSource_DIRECTIONAL)
          numDirLights++;
      
      if (_settings->_directionalSVec.size() > numDirLights)
      {
        intensity = _settings->_directionalSVec[numDirLights].intensity;
        castsShadow = _settings->_directionalSVec[numDirLights].castsShadow;
      }
      else
      {
        intensity = 1.f;
        castsShadow = true;
      }

      light = new Directional(spec, Vector(ailight->mDirection.x,
                                           ailight->mDirection.y,
                                           ailight->mDirection.z), intensity );
      GBin.mrkDel<Light*>(light);

      return light;
    }
    // Puntklichter erzeugen
    case aiLightSource_POINT:
    {
      uint numPointLights = -1;

      for (uint j=0; j<=i; j++)
        if (scene->mLights[j]->mType == aiLightSource_POINT)
          numPointLights++;

      if (_settings->_pointLightSVec.size() > numPointLights)
      {
        intensity = _settings->_pointLightSVec[numPointLights].intensity;
        castsShadow = _settings->_pointLightSVec[numPointLights].castsShadow;
      }
      else
      {
        intensity = 1.f;
        castsShadow = true;
      }

      light = new PointLight(Point(ailight->mPosition.x, ailight->mPosition.y,
                                   ailight->mPosition.z), spec, intensity);
      GBin.mrkDel<Light*>(light);

      return light;
    }
    // Flächenlichter erzeugen
    case aiLightSource_SPOT:
    {
      uint numAreaLights = -1;
      Emissive* emissiveMaterial;
      UnitSampler* uSampler;
      LightShape* lightShape;
      bool showLightShape;
      float photonIntensity;
      float discRadius;

      for (uint j=0; j<=i; j++)
        if (scene->mLights[j]->mType == aiLightSource_SPOT)
          numAreaLights++;

      if (_settings->_areaLightSVec.size() > numAreaLights)
      {
        intensity = _settings->_areaLightSVec[numAreaLights].intensity;
        photonIntensity = _settings->_areaLightSVec[numAreaLights].photonIntensity;
        uSampler = _settings->_areaLightSVec[numAreaLights].uSampler;
        lightShape = _settings->_areaLightSVec[numAreaLights].lightShape;
        showLightShape = 
          _settings->_areaLightSVec[numAreaLights].showLightShape;
        castsShadow = _settings->_areaLightSVec[numAreaLights].castsShadow;
        discRadius = _settings->_areaLightSVec[numAreaLights].discRadius;
      }
      else
      {
        intensity = 5.5f;
        photonIntensity = 5.5f;
        uSampler = NULL;
        lightShape = NULL;
        showLightShape = true;
        castsShadow = true;
        discRadius = 1.f;
      }

      if (uSampler == NULL)
      {
        uSampler = new StratifiedS(4);
        GBin.mrkDel<UnitSampler*>(uSampler);
      }

      if (lightShape == NULL)
      {
        lightShape = new Disc(Point(ailight->mPosition.x, ailight->mPosition.y,
                                    ailight->mPosition.z), discRadius, 
                              Normal(ailight->mDirection.x,
                                     ailight->mDirection.y,
                                     ailight->mDirection.z) );
        GBin.mrkDel<Shape*>(lightShape);
      }
      
      Spectrum spec;
      spec[2] = ailight->mColorDiffuse.r;
      spec[1] = ailight->mColorDiffuse.g;
      spec[0] = ailight->mColorDiffuse.b;
      
      emissiveMaterial = new Emissive;
      GBin.mrkDel<Material*>(emissiveMaterial);
                                    
      light = new AreaLight(intensity, photonIntensity, spec, lightShape, emissiveMaterial,
                            uSampler, *_world,
                            showLightShape, castsShadow);
      GBin.mrkDel<Light*>(light);

      return light;
    }
  }


  return NULL;
}


//_____________________________________________________________________________
Material*
Parser::createMaterial(const stdstring& sceneFile, const aiScene* sc,
const aiMesh* aimesh)
{
  float specExp, specStrength;
  Texture* ambientT, * diffuseT, * specularT, * normalT;
  Material* material;
  NormalMap* normalMap = NULL;
  aiMaterial* aimaterial;
  Spectrum ambientS, diffuseS, specularS;
  aiColor3D aicolor3D(0.f,0.f,0.f);


  aimaterial = sc->mMaterials[aimesh->mMaterialIndex];

  // TODO: Bug in Assimp, sobald man collada (.dae) verwendet wird die ambient
  // Texture durch aiTextureType_LIGHTMAP geladen. Entweder selber fixen oder
  // hoffen, dass das irgendwann die Assimp Leute tun
  ambientT = createTexture(sceneFile, sc, aimesh, aiTextureType_LIGHTMAP);
  diffuseT = createTexture(sceneFile, sc, aimesh, aiTextureType_DIFFUSE);
  specularT= createTexture(sceneFile, sc, aimesh, aiTextureType_SPECULAR);
  normalT  = createTexture(sceneFile, sc, aimesh, aiTextureType_NORMALS);

  aimaterial->Get(AI_MATKEY_SHININESS, specExp);
  specExp *= 100; // OpenCALLADA teilt den Wert durch 100
  aimaterial->Get(AI_MATKEY_SHININESS_STRENGTH, specStrength);

  aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, aicolor3D);
  ambientS[2] = aicolor3D.r;
  ambientS[1] = aicolor3D.g;
  ambientS[0] = aicolor3D.b;
  
  aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aicolor3D);
  diffuseS[2] = aicolor3D.r;
  diffuseS[1] = aicolor3D.g;
  diffuseS[0] = aicolor3D.b;

  aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, aicolor3D);
  specularS[2] = aicolor3D.r;
  specularS[1] = aicolor3D.g;
  specularS[0] = aicolor3D.b;

  if (normalT != NULL)
  {
    normalMap = new NormalMap(normalT);
    GBin.mrkDel<NormalMap*>(normalMap);
  }
  material = new AssimpMaterial(ambientS, diffuseS, specularS, specExp,
                                ambientT, diffuseT, specularT, normalMap);
  GBin.mrkDel(material);
 

  return material;
}


//_____________________________________________________________________________
Texture*
Parser::createTexture(const stdstring& sceneFile, const aiScene* sc,
                      const aiMesh* aimesh, aiTextureType texType)
{
  if (sc->HasTextures() )
    WOUT("Szenen mit impliziten Texturen werden nicht unterstützt");

  Texture* imageTex;
  aiMaterial* aimaterial;
  uint numTextures;
  

  aimaterial = sc->mMaterials[aimesh->mMaterialIndex];
  numTextures = aimaterial->GetTextureCount(texType);

  if (numTextures > 1)
  {
    WOUT("Es wird zur Zeit nicht mehr als 1 Textur pro Lichtkomponente (Phong)"
         " unterstützt");
  }


  /*for (uint i=0; i<numDiffTextures; i++)*/
  if (numTextures > 0)
  {
    aiString path;
    char drive[_MAX_DRIVE];
    char dirname[_MAX_DIR];
    wchar_t wpath[256];
    std::vector<std::vector<RGBASpectrum> > pxlData;
    aiReturn ret  = aimaterial->GetTexture(texType, 0, &path);


    if (ret != aiReturn_SUCCESS)
    {
      EOUT("Beim Anfordern der Textur von Assimp lief ewtas schief");
      std::cin.get();
      exit(1);
    }


    // Überprüfe ob es sich um einen relativen Pfad handelt
#ifdef _UNICODE
  size_t charsConverted;
  char sceneFileMBS[265];
  wcstombs_s(&charsConverted, sceneFileMBS, sceneFile.c_str(), 256);
#else
  const char* sceneFileMBS = sceneFile.c_str();
#endif //_UNICODE
    _splitpath_s(path.C_Str(), drive, _MAX_DRIVE, NULL, 0, NULL, 0, NULL, 0);
    _splitpath_s(sceneFileMBS, NULL, 0, dirname, _MAX_DIR, NULL, 0,
                 NULL, 0);
    if (drive[0] == 0)
    {
      char tmp[265];
      sprintf_s(tmp, "%s%s", dirname, path.C_Str() );
      sprintf_s(path.data, "%s", tmp);
    }
    MultiByteToWideChar(CP_ACP, 0, path.data, -1, wpath, 265);

    IMAGEIO::readImage(wpath, pxlData);


    imageTex = new ImageTex(path.C_Str(), pxlData);
    GBin.mrkDel(imageTex);

    
    return imageTex;
  }


  return NULL;
}


//_____________________________________________________________________________
void
Parser::parseSceneFromFile(const stdstring& sceneFile)
{
  Camera*   camera;
  Light*    light;

  
  if (sceneFile == _T("") )
  {
    IOUT("Es wurde keine Scenefile angegeben.");
    return;
  }
  
  //check if file exists
	std::ifstream fin(sceneFile);
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		EOUT(_T("Die Datei \"") << sceneFile << _T("\" ist nicht vorhanden.") );
    std::cin.get();
    exit(1);
	}

  Assimp::Importer importer;
#ifdef _UNICODE
  size_t charsConverted;
  char sceneFileMBS[265];
  wcstombs_s(&charsConverted, sceneFileMBS, sceneFile.c_str(), 256);
#else
  const char* sceneFileMBS = sceneFile.c_str();
#endif //_UNICODE
#ifdef _DEBUG
  const aiScene* scene = importer.ReadFile(sceneFileMBS, 
                                           aiProcess_JoinIdenticalVertices |
                                           aiProcess_Triangulate |
                                           aiProcess_GenSmoothNormals |
                                           aiProcess_PreTransformVertices |
                                           aiProcess_ValidateDataStructure |
                                           aiProcess_RemoveRedundantMaterials |
                                           aiProcess_FlipUVs);
#else
  const aiScene* scene = importer.ReadFile(sceneFileMBS, 
                                           aiProcess_JoinIdenticalVertices |
                                           aiProcess_Triangulate |
                                           aiProcess_GenSmoothNormals |
                                           aiProcess_PreTransformVertices |
                                           aiProcess_RemoveRedundantMaterials |
                                           aiProcess_FlipUVs);
#endif

	// If the import failed, report it
  if( !scene)
  {
    EOUT(importer.GetErrorString() );
    std::cin.get();
    exit(1);
  }


  // Erzeuge die Kamera. Zur Zeit wird nur eine einzige Kamera unterstützt
  camera   = createCamera(sceneFile, scene);
  _world->addCamera(camera);

  // Erzeuge die Lichter.
  for (uint i=0; i<scene->mNumLights; i++)
  {
    light = createLights(sceneFile, scene, i);
    _world->addLight(light);
  }


  parseRecursive(sceneFile, scene, scene->mRootNode);

}


//_____________________________________________________________________________
void
Parser::parseRecursive(const stdstring& sceneFile, const aiScene* sc,
                       const aiNode* nd)
{
  unsigned int numMeshes = nd->mNumMeshes;


  for (uint i=0; i<nd->mNumMeshes; i++)
  {
    Material* material;
    Mesh mesh;

    material = createMaterial(sceneFile, sc, sc->mMeshes[nd->mMeshes[i] ]);
    fillMesh(mesh, sceneFile, sc, sc->mMeshes[nd->mMeshes[i] ]);
    AssimpMeshTriangle* assimpMT = new AssimpMeshTriangle(mesh);
    wchar_t meshName[256];
    auto assimpStr = sc->mMeshes[nd->mMeshes[i] ]->mName;
    mbstowcs_s(NULL, meshName, assimpStr.C_Str(), assimpStr.length);
    assimpMT->setName(meshName);
    assimpMT->setMaterial(material);
    assimpMT->setAccelerator(_settings->_globalAccelerator->newInstance() );
    Instance* instance = new Instance(assimpMT);

    _world->addShape(instance);
    
    GBin.mrkDel<Shape*>(assimpMT);
    GBin.mrkDel<Shape*>(instance);
  }



  for (uint i=0; i<nd->mNumChildren; i++)
  {
    parseRecursive(sceneFile, sc, nd->mChildren[i]);
  }

}
