#include "stdafx.h"


//#include <random>
//
//#include "mirror.h"
//#include "mutexedrng.h"
//#include "photon.h"
//#include "refprobability.h"

//_____________________________________________________________________________
Mirror::Mirror(void)
  : Phong(), _pSpecularBRDF(new PerfectSpecular)
{
}


//_____________________________________________________________________________
Mirror::Mirror(Spectrum objC, Spectrum specC, Spectrum perfSpecC, 
               double ambientRefl, double diffuseRefl, double specularRefl, 
               double exp, double perfectSpecularRefl)
 : Phong(objC, specC, ambientRefl, diffuseRefl, specularRefl, exp), 
   _pSpecularBRDF(new PerfectSpecular(perfSpecC, perfectSpecularRefl) )
{
}


//_____________________________________________________________________________
Mirror::~Mirror(void)
{
  delete _pSpecularBRDF;
  _pSpecularBRDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//Mirror::shade(ShadeRec& sr) const
//{
//  Spectrum L(Phong::shade(sr) );  //direct illumination
//
//  Vector wo = -1. * sr.ray.d();
//  Vector wi;
//  Spectrum refL = _pSpecularBRDF->sampleF(sr, wi, wo);
//  Ray refRay(sr.hitPoint, wi);
//
//  L += refL * sr.world.renderer->Li(refRay, sr.depth + 1) *
//       static_cast<float>(sr.normal * wi);
//
//  return L;
//}


//_____________________________________________________________________________
Spectrum
Mirror::areaLightShade(ShadeRec& sr) const
{
  Spectrum L(Phong::areaLightShade(sr) );  //direct illumination
  float dummy;

  Vector wo = -1. * sr.ray.d();
  Vector wi;
  Spectrum refL = _pSpecularBRDF->sampleF(sr, wi, wo, dummy);
  Ray refRay(sr.hitPoint, wi);
  
 
  L += refL * 
       (
         sr.world.renderer->Li(refRay, sr.depth + 1) +  // Direkte Beleuchtung behandeln
         sr.world.globalRenderer->Li(refRay, 0)         // Globale Beleuchtung behandeln
       ) *
       static_cast<float>(sr.normal * wi);


  return L;
}


//_____________________________________________________________________________
Spectrum
Mirror::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  // TODO:: Später soll hier einfach photonShade der Basisklasse Phong benutzt
  // werden.
  Vector dummy1, dummy2;


  return Phong::photonShade(sr, photonDir);
}


//_____________________________________________________________________________
bool Mirror::tracePhoton(const TracePhoton& curPhoton, 
                         TracePhoton& tracedPhoton,
                         const ShadeRec& sr) const
{
  // TODO: Lichtreflex mit Phong-lobe wurde noch nicht behandelt

  float dummy;
  Vector wo;
  std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  const Vector wi = -1. * curPhoton.photonRay.d();

  Spectrum diffRefL = _diffuseBRDF->getSpectrum() * 
                      static_cast<float>(_diffuseBRDF->getReflectance() );
  Spectrum pspecRefL = _pSpecularBRDF->photonSample(sr, wi, wo, dummy);
  float probDiffRefL = RefProbability::getProb(diffRefL);
  float probPSpecRefL = RefProbability::getProb(pspecRefL);
  float randProb = MutexedRNG::number(uniformDist);


  // Photon wird absorbiert
  if (randProb > probDiffRefL + probPSpecRefL)
  {
    tracedPhoton = curPhoton;
    tracedPhoton.photonRay = Ray(sr.hitPoint, curPhoton.photonRay.d() );
    tracedPhoton.onlySpecularRefl &= true;

    return true;
  }
  // Photon wird perfekt reflektiert
  else if (randProb > probDiffRefL)
  {
    tracedPhoton.color = curPhoton.color * pspecRefL / probPSpecRefL;
    tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
    tracedPhoton.onlySpecularRefl &= true;  // eigentlich unnötig, aber so weiß man was geschehen soll

    return false;
  }
  // Photon wird diffus reflektiert
  else
  {
    diffRefL = _diffuseBRDF->photonSample(sr, wi, wo, dummy);
    tracedPhoton.color = curPhoton.color * diffRefL;
    tracedPhoton.photonRay = Ray(sr.hitPoint, wo );
    tracedPhoton.onlySpecularRefl = false;


    return false;
  }


  return false;
}