#pragma once

#ifndef ANTIALIASINGSETTINGS_H_
#define ANTIALIASINGSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class AntialiasingSettings : public Settings
{
public:
  AntialiasingSettings(Options& options);
  virtual ~AntialiasingSettings(void);
};

#endif //ANTIALIASINGSETTINGS_H_