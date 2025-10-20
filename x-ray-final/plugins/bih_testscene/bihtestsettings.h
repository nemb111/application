#pragma once

#ifndef BIHTESTSETTINGS_H_
#define BIHTESTSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class BIHTestSettings : public Settings
{
public:
  BIHTestSettings(Options& options);
  virtual ~BIHTestSettings(void);
};

#endif //BIHTESTSETTINGS_H_