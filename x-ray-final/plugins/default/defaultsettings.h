#pragma once

#ifndef DEFAULTSETTINGS_H_
#define DEFAULTSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class DefaultSettings : public Settings
{
public:
  DefaultSettings(Options& options);
  virtual ~DefaultSettings(void);
};

#endif //DEFAULTSETTINGS_H_