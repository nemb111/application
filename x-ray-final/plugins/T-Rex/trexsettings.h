#pragma once

#ifndef TREXSETTINGS_H_
#define TREXSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class TrexSettings : public Settings
{
public:
  TrexSettings(Options& options);
  virtual ~TrexSettings(void);
};

#endif //TREXSETTINGS_H_