#pragma once

#ifndef BIHBENCHMARKSETTINGS_H_
#define BIHBENCHMARKSETTINGS_H_

#include "settings.h"

//Forward Deklarationen
class Options;

class BIHBenchmarkSettings : public Settings
{
public:
  BIHBenchmarkSettings(Options& options);
  virtual ~BIHBenchmarkSettings(void);
};

#endif //BIHBENCHMARKSETTINGS_H_