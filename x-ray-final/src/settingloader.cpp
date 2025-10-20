#include "stdafx.h"

//#include "options.h"
//#include "settingloader.h"
//#include "settings.h"
//#include "x-ray.h"

//_____________________________________________________________________________
SettingLoader::SettingLoader(const Options& options)
{
  stdstring dllFile = options.settingDLL;

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
SettingLoader::~SettingLoader(void)
{
}


//_____________________________________________________________________________
Settings*
SettingLoader::load(const Options& options)
{
  _hInstLibrary = LoadLibrary(_dllFile.c_str() );
 
  if (_hInstLibrary)
  {
    CreateSettings createSettings;
    createSettings = (CreateSettings)GetProcAddress(_hInstLibrary, "createSettings");
 
    if (!createSettings)
    {
      WOUT(_T("Die Plugin-Funktion \"createSettings\" wird von der geladenen DLL ")
           _T("nicht exportiert.") << std::endl);
      FreeLibrary(_hInstLibrary);

      return NULL;
    }
    _settings = createSettings(options);
     
    return _settings;
  }
  else
  {
    IOUT(_T("Die DLL \"") << _dllFile << _T("\" konnte nicht geladen werden.") 
         << _T(" Lade Default-Settings (\"default.dll\").") );

    _hInstLibrary = LoadLibrary(_T("default.dll") );

    if (_hInstLibrary)
    {
      CreateSettings createSettings;
      createSettings = (CreateSettings)GetProcAddress(_hInstLibrary, "createSettings");

      if (!createSettings)
      {
        WOUT(_T("Die Plugin-Funktion \"createSettings\" wird von der geladenen DLL ")
             _T("nicht exportiert.") << std::endl);
        FreeLibrary(_hInstLibrary);

        return NULL;
      }

      _settings = createSettings(options);

      return _settings;
    }
    else
    {
      EOUT(_T("Konnte das Plugin \"default.dll\" nicht laden.") );
      std::wcin.get();
      exit(1);

      return NULL;
    }

    return NULL;
  }
}


//_____________________________________________________________________________
void
SettingLoader::unload()
{

}