#include "stdafx.h"

#include "stopwatch.h"


//_____________________________________________________________________________
static bool optionParsed = 0;


//_____________________________________________________________________________
static HANDLE randUIMutex = CreateMutex(NULL, FALSE, NULL);


//_____________________________________________________________________________
double randUI()
{
  int mutex_result = WaitForSingleObject(randUIMutex, //handle to mutex
                                         INFINITE);    //no time-out interval
#ifdef _DEBUG
  switch (mutex_result) 
  {
    // The 	acquiring of the mutex failed. Possible problem: mutex not
    // initialized
    case WAIT_FAILED:
    {
      EOUT("Der Mutex konnte nicht erworben werden");
      std::cin.get();
      exit(1);
    }
    // The thread got ownership of the mutex
    case WAIT_OBJECT_0:
      break; 
    // The thread got ownership of an abandoned mutex
    // The database is in an indeterminate state
    case WAIT_ABANDONED:
      WOUT("Es wurde ein \"abandoned mutex\" erworben.");
      return FALSE; 
  }
#endif

  double res = rand() / (double)RAND_MAX;


#ifndef NDEBUG
  if (ReleaseMutex(randUIMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(randUIMutex);
#endif //NDEBUG


  return res;
}


//_____________________________________________________________________________
int parseOptions(int argc, wchar_t** argv, Options& options)
{

  
  // TODO : Verwenden von unicode anstelle von ASCII
  // Verwende Unicode (benötigt u.a. wcout)
  //_setmode(_fileno(stdout), _O_U16TEXT);

  Stopwatch::start(_T("X-ray") );

  // Process command-line arguments
  for (int i = 0; i < argc; ++i)
  {
    if (!wcscmp(argv[i], _T("--ncores") )) options.nCores = _wtoi(argv[++i]);
    else if (!wcscmp(argv[i], _T("--resolution") ))
    {
      options.xResolution = _wtoi(argv[++i]);
      options.yResolution = _wtoi(argv[++i]);
    }
    else if (!wcscmp(argv[i], _T("--debugPixel") ) )
    {
      options.debugPixelX = _wtoi(argv[++i]);
      options.debugPixelY = _wtoi(argv[++i]);
    }
    else if (!wcscmp(argv[i], _T("--sceneDLL") ) ) options.sceneDLL = argv[++i];
    else if (!wcscmp(argv[i], _T("--settingDLL") ) ) options.settingDLL = argv[++i];
    else if (!wcscmp(argv[i], _T("--outfile") )) options.outfile = argv[++i];
    else if (!wcscmp(argv[i], _T("--verbose") )) options.verbose = _wtoi(argv[++i]);
    else if (!wcscmp(argv[i], _T("--window") ))  options.window = true;
    else if (!wcscmp(argv[i], _T("--disableDirIll") )) options.dirIll = false;
    else if (!wcscmp(argv[i], _T("--disableGlobIll") )) options.globalIll = false;
    else if (!wcscmp(argv[i], _T("--random") )) time(&options.seed );
    else if (!wcscmp(argv[i], _T("--help") ) || !wcscmp(argv[i], _T("-h") ) || argc==1)
    {
        printf("Usage: x-ray [--ncores n] [--outfile filename] "
                "[--verbose n] [--help || -h] <filename> ...\n");
        return 0;
    }
    else if (!wcscmp(argv[i], _T("--infile") )) options.infile = argv[++i];
  }
  optionParsed = true;
  srand((uint) options.seed);
  MutexedRNG::seed(static_cast<unsigned long>(options.seed) );

  return 1;

}


//_____________________________________________________________________________
int start_xray(GUIcomm* guiComm/*=NULL*/)
{
  Parser parser;
  Options& options = Options::getInstance();


  if (parser.createSettings() && parser.createWorld(options.infile, guiComm) )
    parser.startRendering();

  parser.destroySettings();
  parser.destroyWorld();


  IOUT(_T("Laufzeit des Programms: ") << Stopwatch::end(_T("X-ray") )
       / 1000. << _T("s") );
  
  return 0;
}


//_____________________________________________________________________________
int start_xray(int argc, wchar_t** argv, GUIcomm* guiComm/*=NULL*/)
{
  Parser parser;
  Options& options = Options::getInstance();


  if(!optionParsed)
    if(parseOptions(argc, argv, options) )
      return 0;

  if (parser.createSettings() && parser.createWorld(options.infile, guiComm) )
    parser.startRendering();

  parser.destroySettings();
  parser.destroyWorld();

  
  IOUT(_T("Laufzeit des Programms: ") << 
       Stopwatch::end(_T("X-ray") ) / 1000. << _T("s") );

  return 0;
}
