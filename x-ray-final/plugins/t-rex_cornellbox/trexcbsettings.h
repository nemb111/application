#pragma once


#ifndef TREXCBSETTINGS_H_
#define TREXCBSETTINGS_H_

#include "settings.h"

class TrexCBSettings : public Settings
{
public:
  TrexCBSettings(Options& options);
  virtual ~TrexCBSettings(void);
};


#endif //TREXCBSETTINGS_H_