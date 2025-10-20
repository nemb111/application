#pragma once


#ifndef MWSETTINGS_H_
#define MWSETTINGS_H_

#include "settings.h"

class MWSettings : public Settings
{
public:
  MWSettings(Options& options);
  virtual ~MWSettings(void);
};


#endif //MWSETTINGS_H_