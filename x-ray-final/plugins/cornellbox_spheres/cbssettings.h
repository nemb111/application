#pragma once


#ifndef CBSSETTINGS_H_
#define CBSSETTINGS_H_

#include "settings.h"

class CBSSettings : public Settings
{
public:
  CBSSettings(Options& options);
  virtual ~CBSSettings(void);
};


#endif //CBSSETTINGS_H_