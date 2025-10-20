#pragma once


#ifndef CBHSETTINGS_H_
#define CBHSETTINGS_H_

#include "settings.h"

class CBHSettings : public Settings
{
public:
  CBHSettings(Options& options);
  virtual ~CBHSettings(void);
};


#endif //CBHSETTINGS_H_