#pragma once


#ifndef FSSSETTINGS_H_
#define FSSSETTINGS_H_

#include "settings.h"

class FSSSettings : public Settings
{
public:
  FSSSettings(Options& options);
  virtual ~FSSSettings(void);
};


#endif //CBHSETTINGS_H_