#pragma once

#ifndef SAMPLESTORAGE_H_
#define SAMPLESTORAGE_H_

#include <map>
#include <string>
#include <vector>

#include "hemispheres.h"
#include "lightshape.h"
#include "unitsampler.h"

//_____________________________________________________________________________
class SampleStorage
{
public:
  SampleStorage(void);
  ~SampleStorage(void);

  const std::vector<const Point>*
    getAreaLightSamples(LightShape* lightShape, uint nPattern=100);
  const std::vector<const std::vector<const double> >*
    getUSquSamples(UnitSampler* sampler, uint nPattern=100);
  const std::vector<const Point>*
    getHSphSamples(HemisphereS* hsphere, uint nPattern=100);
  const std::vector<const Point>*
    getHSphSamples(const HemisphereS* hsphere, uint nPattern=100);

private:



  struct ALightSamples
  {
    std::vector<const Point> points;
  };

  struct USquSamples
  {
    std::vector<const std::vector<const double> > xy;
  };

  struct HSphSamples
  {
    std::vector<const Point> points;
  };

  std::map<std::string, std::vector<ALightSamples> > _aLightStorage;
  std::map<std::string, std::vector<USquSamples> >   _uSquStorage;
  std::map<std::string, std::vector<HSphSamples> >   _hSphStorage;

  HANDLE _aLightMutex;
  HANDLE _uSquMutex;
  HANDLE _hSphMutex;

  void createALightSamples(const std::string& key,
                           LightShape* lightShape, uint nPattern);
  void createUSquSamples(const std::string& key, UnitSampler* sampler,
                         uint nPattern);
  void createHSphSamples(const std::string& key, HemisphereS* hsphere,
                         uint nPattern);
};


//_____________________________________________________________________________
class SStorage
{
public:
  static const std::vector<const Point>*
    getALightSamples(LightShape* lightShape, uint nPattern=100)
  {
    return _sampleStorage.getAreaLightSamples(lightShape, nPattern);
  }
  static const std::vector<const std::vector<const double> >*
    getUSquSamples(UnitSampler* sampler, uint nPattern=100)
  {
    return _sampleStorage.getUSquSamples(sampler, nPattern);
  }
  static const std::vector<const Point>*
    getHSphSamples(HemisphereS* hsphere, uint nPattern=100)
  {
    return _sampleStorage.getHSphSamples(hsphere, nPattern);
  }
  static const std::vector<const Point>*
    getHSphSamples(const HemisphereS* hsphere, uint nPattern=100)
  {
    return _sampleStorage.getHSphSamples(hsphere, nPattern);
  }

private:
  static SampleStorage _sampleStorage;
};



#endif //SAMPLESTORAGE_H_