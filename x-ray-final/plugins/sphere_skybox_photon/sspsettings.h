#pragma once


#ifndef SSPSETTINGS_H_
#define SSPSETTINGS_H_

#include "settings.h"

class SSPSettings : public Settings
{
public:
  SSPSettings(Options& options);
  virtual ~SSPSettings(void);
};


#endif //SSPSETTINGS_H_