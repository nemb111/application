#include "stdafx.h"

//#include <iostream>

//#include "options.h"
//#include "scene.h"
//#include "sceneloader.h"
//#include "x-ray.h"


//_____________________________________________________________________________
SceneLoader::SceneLoader(const Options& options)
  : _hInstLibrary(0), _scene(NULL)
{
  stdstring dllFile = options.sceneDLL;

  if (dllFile == stdstring(_T("") ) )
  {
    cchar dir[512], filename[512];

    // Keine DLL angegeben: Suche nach einer DLL die wie der Ordner heiﬂt,
    // in dem die scene-file liegt
    _wsplitpath_s(options.infile.c_str(), NULL, 0, dir, 512, NULL, 0, NULL, 0);
    size_t i;
    for (i=0; dir[i] != L'\0'; i++);
    dir[i-1] = L'\0';
    _wsplitpath_s(dir, NULL, 0, NULL, 0, filename, 512, NULL, 0);
    _dllFile = stdstring(filename) + stdstring(_T(".dll") );
  }
  else
    _dllFile = dllFile;

}


//_____________________________________________________________________________
SceneLoader::~SceneLoader(void)
{
}


//_____________________________________________________________________________
Scene*
SceneLoader::load(const Options& options)
{
 
  _hInstLibrary = LoadLibrary(_dllFile.c_str() );
 
  if (_hInstLibrary)
  {
    CreateScene createScene;
    createScene = (CreateScene)GetProcAddress(_hInstLibrary, "createScene");
 
    if (!createScene)
    {
      WOUT(_T("Die Plugin-Funktion \"createScene\" wird von der geladenen DLL ")
           _T("nicht exportiert.") << std::endl);
      FreeLibrary(_hInstLibrary);

      return NULL;
    }
    _scene = createScene(options);
     

    return _scene;
  }
  else
  {
    IOUT(_T("Die DLL \"") << _dllFile << _T("\" konnte nicht geladen werden.") );
    return NULL;
  }
}


//_____________________________________________________________________________
void
SceneLoader::unload()
{
  delete _scene;
  _scene = 0;

  if (_hInstLibrary)
    FreeLibrary(_hInstLibrary);
}