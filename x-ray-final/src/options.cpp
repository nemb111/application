#include "stdafx.h"

//#define NOMINMAX

//#include "tchar.h"
//#include "windows.h"
//
//#include <limits>
//
//#include "options.h"


//_____________________________________________________________________________
Options::Options()
  : seed(1), nCores(1), verbose(0), xResolution(1360), yResolution(768),
    debugPixelX(std::numeric_limits<unsigned int>::max() ),
    debugPixelY(std::numeric_limits<unsigned int>::max() ),
    infile(_T("") ), outfile(_T("./out/out.jpg") ),
    sceneDLL(_T("") ), settingDLL(_T("") ),
    window(false), dirIll(true), globalIll(true)
{
}


//_____________________________________________________________________________
Options& Options::operator=(const Options& rhs)
{
  seed          = rhs.seed;
  nCores        = rhs.nCores;
  verbose       = rhs.verbose;
  xResolution   = rhs.xResolution;
  yResolution   = rhs.yResolution;
  debugPixelX   = rhs.debugPixelX;
  debugPixelY   = rhs.debugPixelY;
  infile        = rhs.infile;
  outfile       = rhs.outfile;
  sceneDLL   = rhs.sceneDLL;
  settingDLL = rhs.settingDLL;
  window        = rhs.window;
  dirIll        = rhs.dirIll;      
  globalIll     = rhs.globalIll;

  return (*this);
}


//_____________________________________________________________________________
int numSystemCores(const Options& options)
{
  if (options.nCores > 0)
    return options.nCores;
  else
  {
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );

    return sysinfo.dwNumberOfProcessors;
  }
}
