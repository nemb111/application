#pragma once

#ifndef TRANSPARENCYSETTINGS_H_
#define TRANSPARENCYSETTINGS_H_


#include "settings.h"

class TransparencySettings : public Settings
{
public:
  TransparencySettings(Options& options);
  virtual ~TransparencySettings(void);
};


#endif //TRANSPARENCYSETTINGS_H_