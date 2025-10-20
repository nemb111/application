#pragma once

#ifndef TREXNORMSETTINGS_H_
#define TREXNORMSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class TrexNormSettings : public Settings
{
public:
  TrexNormSettings(Options& options);
  virtual ~TrexNormSettings(void);
};

#endif //TREXNORMSETTINGS_H_