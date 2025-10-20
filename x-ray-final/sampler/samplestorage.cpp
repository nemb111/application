#include "stdafx.h"

//#include "samplestorage.h"


//_____________________________________________________________________________
SampleStorage::SampleStorage(void)
  : _aLightMutex(CreateMutex(NULL, FALSE, NULL) ),
    _uSquMutex(CreateMutex(NULL, FALSE, NULL) ), 
    _hSphMutex(CreateMutex(NULL, FALSE, NULL) )
{
}


//_____________________________________________________________________________
SampleStorage::~SampleStorage(void)
{
}


//_____________________________________________________________________________
const std::vector<const Point>*
SampleStorage::getAreaLightSamples(LightShape* lightShape,
                                   uint nPattern/*=100*/)
{
  std::string key = 
    std::to_string(static_cast<unsigned long long>(lightShape->getId() ) ) +
    std::to_string(static_cast<unsigned long long>(nPattern) );
  std::map<std::string, std::vector<ALightSamples> >::const_iterator cIt;

  int mutex_result = WaitForSingleObject(_aLightMutex, //handle to mutex
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

  cIt = _aLightStorage.find(key);

  // Der berechnete Schlüssel befindet sich nicht in der Map, weshalb die 
  // angeforderten Samples on-the-fly erzeugt werden
  if (cIt == _aLightStorage.end() )
    createALightSamples(key, lightShape, nPattern);
  
  cIt = _aLightStorage.find(key);
    

  // die angeforderten Samples wurden bereits erzeugt. Gebe den Pointer darauf
  // zurück.
  size_t idx = rand() % nPattern;
  auto* res = &(cIt->second[idx].points);
  
#ifndef NDEBUG
  if (ReleaseMutex(_aLightMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_aLightMutex);
#endif

  
  return res;
}


//_____________________________________________________________________________
const std::vector<const std::vector<const double> >* 
SampleStorage::getUSquSamples(UnitSampler* sampler, uint nPattern/*=100*/)
{

  std::string key = sampler->className() + 
    std::to_string(static_cast<unsigned long long>(sampler->getNumSamples() ) )+
    std::to_string(static_cast<unsigned long long>(nPattern) );

  int mutex_result = WaitForSingleObject(_uSquMutex,      //handle to mutex
                                         INFINITE);   //no time-out interval

#ifndef NDEBUG
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
#endif //NDEBUG


  std::map<std::string, std::vector<USquSamples> >::const_iterator cIt =
    _uSquStorage.find(key);

  // Der berechnete Schlüssel befindet sich nicht in der Map, weshalb die 
  // angeforderten Samples on-the-fly erzeugt werden
  if (cIt == _uSquStorage.end() )
    createUSquSamples(key, sampler, nPattern);

  cIt = _uSquStorage.find(key);

  

  // die angeforderten Samples wurden bereits erzeugt. Gebe den Pointer darauf
  // zurück.
  size_t idx = rand() % nPattern;
  auto* res = &(cIt->second[idx].xy);

#ifndef NDEBUG
  if (ReleaseMutex(_uSquMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_uSquMutex);
#endif //NDEBUG


  return res;
}


//_____________________________________________________________________________
const std::vector<const Point>*
SampleStorage::getHSphSamples(HemisphereS* hsphere, uint nPattern/*=100*/)
{
  std::string key = hsphere->className() + 
    std::to_string(static_cast<unsigned long long>(hsphere->getNumSamples() ) )+
    std::to_string(static_cast<unsigned long long>(nPattern) ) +
    std::to_string(static_cast<long double>(hsphere->getE() ) );


  int mutex_result = WaitForSingleObject(_hSphMutex,      //handle to mutex
                                         INFINITE);   //no time-out interval

#ifndef NDEBUG
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
#endif //NDEBUG

  
  std::map<std::string, std::vector<HSphSamples> >::const_iterator cIt =
    _hSphStorage.find(key);

  // Der berechnete Schlüssel befindet sich nicht in der Map, weshalb die 
  // angeforderten Samples on-the-fly erzeugt werden
  if (cIt == _hSphStorage.end() )
    createHSphSamples(key, hsphere, nPattern);

  cIt = _hSphStorage.find(key);



  // die angeforderten Samples wurden bereits erzeugt. Gebe den Pointer darauf
  // zurück.
  size_t idx = rand() % nPattern;
  auto* res = &(cIt->second[idx].points);

#ifndef NDEBUG
  if (ReleaseMutex(_hSphMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_hSphMutex);
#endif//NDEBUG


  return res;
}


//_____________________________________________________________________________
const std::vector<const Point>*
SampleStorage::getHSphSamples(const HemisphereS* hsphere, 
                              uint nPattern/*=100*/)
{
    std::string key = hsphere->className() + 
    std::to_string(static_cast<unsigned long long>(hsphere->getNumSamples() ) )+
    std::to_string(static_cast<unsigned long long>(nPattern) ) +
    std::to_string(static_cast<long double>(hsphere->getE() ) );


  int mutex_result = WaitForSingleObject(_hSphMutex,      //handle to mutex
                                         INFINITE);   //no time-out interval

#ifndef NDEBUG
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
#endif //NDEBUG

  
  std::map<std::string, std::vector<HSphSamples> >::const_iterator cIt =
    _hSphStorage.find(key);

  // Der berechnete Schlüssel befindet sich nicht in der Map, weshalb die 
  // angeforderten Samples on-the-fly erzeugt werden
  if (cIt == _hSphStorage.end() )
  {
    HemisphereS* tmp = hsphere->clone();
    createHSphSamples(key, tmp, nPattern);
    delete tmp;
  }

  cIt = _hSphStorage.find(key);


  // die angeforderten Samples wurden bereits erzeugt. Gebe den Pointer darauf
  // zurück.
  size_t idx = rand() % nPattern;
  auto* res = &(cIt->second[idx].points);

#ifndef NDEBUG
  if (ReleaseMutex(_hSphMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_hSphMutex);
#endif//NDEBUG


  return res;
}


//_____________________________________________________________________________
void
SampleStorage::createALightSamples(const std::string& key,
                                   LightShape* lightShape, uint nPattern)
{
  uint nSamples = lightShape->numSamples();
  std::vector<ALightSamples> value;
  value.reserve(nPattern);

  for (uint i=0; i<nPattern; i++)
  {
    if (lightShape->numSamplesCreated() > 0)
      lightShape->resetSampler();
    ALightSamples pattern;
    pattern.points.reserve(nSamples);

    for (uint j=0; j<nSamples; j++)
    {
      Point point = lightShape->getSample();

      pattern.points.push_back(point);
    }

    value.push_back(pattern);
  }

  _aLightStorage[key] = value;
}


//_____________________________________________________________________________
void
SampleStorage::createUSquSamples(const std::string& key, UnitSampler* sampler, 
                                 uint nPattern)
{
 
  uint nSamples = sampler->getNumSamples();
  std::vector<USquSamples> value;
  value.reserve(nPattern);

  for (uint i=0; i<nPattern; i++)
  {
    if (sampler->numSamplesCreated() > 0)
      sampler->reset();
    USquSamples pattern;
    pattern.xy.reserve(nSamples);
    for (uint j=0; j<nSamples; j++)
    {
      double x, y;
      std::vector<const double> xy;
      sampler->createSample(x, y);
      xy.push_back(x);
      xy.push_back(y);

      pattern.xy.push_back(const_cast<std::vector<const double>& >(xy) );
    }

    value.push_back(pattern);
  }

  _uSquStorage[key] = value;
}


//_____________________________________________________________________________
void
SampleStorage::createHSphSamples(const std::string& key, HemisphereS* hsphere,
                                 uint nPattern)
{
  uint nSamples = hsphere->getNumSamples();
  std::vector<HSphSamples> value;
  value.reserve(nPattern);

  for (uint i=0; i<nPattern; i++)
  {
    std::vector<Point> points;
    HSphSamples hSphSamples;
    hsphere->createSamples(points);

    for (size_t j=0; j<points.size(); j++)
      hSphSamples.points.push_back(const_cast<const Point&>(points[j]) );


    value.push_back(hSphSamples);
  }

  _hSphStorage[key] = value;
}


//_____________________________________________________________________________
SampleStorage SStorage::_sampleStorage;