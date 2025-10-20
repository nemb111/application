
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <ctime>
#include <string>

class Options
{
#ifndef _UNICODE
typedef std::string stdstring;
#else
typedef std::wstring stdstring;
#endif

public:

  static Options& getInstance()
  {
    static Options instance;

    return instance;
  }

  time_t seed;
  int nCores;
  int verbose;          
  unsigned int xResolution;
  unsigned int yResolution;
  unsigned int debugPixelX;
  unsigned int debugPixelY;
  stdstring infile;
  stdstring outfile;
  stdstring sceneDLL;
  stdstring settingDLL;
  bool window;
  bool dirIll;                // Use direct Illumination
  bool globalIll;             // Use global Illumination


private:
  Options();
  Options(const Options& rhs);
  Options& operator=(const Options& rhs);


  friend class Scene;
  friend class Settings;
};

int numSystemCores(const Options& options);


#endif//OPTIONS_H_