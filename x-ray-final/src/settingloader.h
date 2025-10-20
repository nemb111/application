#pragma once

#ifndef SETTINGLOADER_H_
#define SETTINGLOADER_H_

#include "x-ray.h"

//Forward Deklarationen
class Options;
class Settings;


class SettingLoader
{
  typedef Settings* (*CreateSettings)(const Options& options);

public:
  SettingLoader(const Options& options);
  virtual ~SettingLoader(void);

  Settings* load(const Options& options);
  void unload();

private:
  stdstring _dllFile;
  HINSTANCE _hInstLibrary;
  Settings* _settings;

};


#endif //SETTINGLOADER_H_