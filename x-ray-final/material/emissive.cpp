#include "stdafx.h"

//#include <random>
//
//#include "emissive.h"
//#include "hemispheres.h"
//#include "mutexedrng.h"
//#include "parser.h"
//#include "photon.h"
//#include "samplestorage.h"
//#include "stratifieds.h"


//_____________________________________________________________________________
Emissive::Emissive(void)
  : _lsFront(1.f), _lsBack(1.f), _lsFrontGlobIl(0.f), _ceFront(WHITE),
  _ceBack(MAGENTA), _ceFrontGlobIl(BLACK)
{
  _unitSampler = new StratifiedS(100);
   Parser::GB.mrkDel<UnitSampler*>(_unitSampler);
  _hemisphereS = HemisphereS(_unitSampler, 1.0);
}


//_____________________________________________________________________________
Emissive::Emissive(float intensityFront, const Spectrum& colorFront,
                   float intensityBack, const Spectrum& colorBack,
                   UnitSampler* unitSampler/*=nullptr*/)
 : _lsFront(intensityFront), _lsBack(intensityBack), _lsFrontGlobIl(0.f),
   _ceFront(colorFront), _ceBack(colorBack), _ceFrontGlobIl(BLACK)
{
  _hemisphereS = HemisphereS(unitSampler, 1.0);
}

//_____________________________________________________________________________
Emissive::~Emissive(void)
{
}


//_____________________________________________________________________________
//Spectrum
//Emissive::shade(ShadeRec& sr) const
//{
//  if ((-1 * sr.normal) * sr.ray.d() > 0.0)
//    return (_lsFront * _ceFront);
//  else
//    return (_lsBack * _ceBack);
//}


//_____________________________________________________________________________
Spectrum
Emissive::areaLightShade(ShadeRec& sr) const
{
  if ((-1 * sr.normal) * sr.ray.d() > 0.0)
    return (_lsFront * _ceFront);
  else
    return (_lsBack * _ceBack);
}


//_____________________________________________________________________________
Spectrum
Emissive::globalShade(ShadeRec& sr) const
{
  // Entweder hat der Kamerastrahl das Licht direkt getroffen oder es handelt
  // sich um den ersten Strahl nachdem ein Objekt getroffen wurde. Die direkte
  // Beleuchtung darf nich doppelt eingehen.
  if (sr.depth <= 1)
    return BLACK;
  else if ((-1 * sr.normal) * sr.ray.d() > 0.0)
    return _lsFrontGlobIl * _ceFrontGlobIl;

  return BLACK;
}


//_____________________________________________________________________________
Spectrum
Emissive::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  return BLACK;
}


//_____________________________________________________________________________
bool
Emissive::tracePhoton(const TracePhoton& curPhoton, TracePhoton& tracedPhoton,
                      const ShadeRec& sr) const
{
  //Photonen die das Licht treffen werden im Moment noch nicht berücksichtigt
  // Setze Farbe wie auch intensität auf 0 um das Photon frühzeitig zu
  // entfernen
 /* tracedPhoton.color = BLACK;
  tracedPhoton.intensity = 0;*/
  //tracedPhoton = curPhoton;

  Normal w = sr.normal;
  Vector u(-1.8473, 0.3183, 2.94729), v;
 
  orthNormBasis(w, u, v);

  const std::vector<const Point>* sp = 
    SStorage::getHSphSamples(&_hemisphereS, 1000);
  std::uniform_int_distribution<int> intDist(0, 
      static_cast<int>( sp->size() - 1) );
  int sampleIdx = MutexedRNG::number(intDist);


  Normal wo = (*sp)[sampleIdx].x * u + (*sp)[sampleIdx].y * v + 
              (*sp)[sampleIdx].z * w;

  tracedPhoton.photonRay = Ray(sr.hitPoint,  wo);


  return false;
}