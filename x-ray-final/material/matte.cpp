#include "stdafx.h"

//#include <random>
//
//#include "matte.h"
//#include "photonrenderer.h"
//#include "mutexedrng.h"
//#include "refprobability.h"

//_____________________________________________________________________________
Matte::Matte(void)
  : _ambientBRDF(new Lambertian(WHITE, 0.05) ), _diffuseBRDF(new Lambertian() )
{
}


//_____________________________________________________________________________
Matte::Matte(Spectrum spectrum, double ambientRefl, double diffuseRefl)
  : _ambientBRDF(new Lambertian(spectrum, ambientRefl) ), 
    _diffuseBRDF(new Lambertian(spectrum, diffuseRefl) )
{
}


//_____________________________________________________________________________
Matte::Matte(const Spectrum& ambientS, const Spectrum& diffuseS)
  : _ambientBRDF(new Lambertian(ambientS, 1.) ),
    _diffuseBRDF(new Lambertian(diffuseS, 1.) )
{
}


//_____________________________________________________________________________
Matte::~Matte(void)
{
  delete _ambientBRDF;
  _ambientBRDF = 0;
  delete _diffuseBRDF;
  _diffuseBRDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//Matte::shade(ShadeRec& sr) const
//{
//  Vector wo = -1 * sr.ray.d();
//  Spectrum resL = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);
//  size_t nLights = sr.world.lights.size();
//  size_t   nSamples;
//  Light* light;
//
//  for (int j = 0; j < nLights; j++)
//  {
//    Spectrum lightL = BLACK;
//    light = sr.world.lights[j];
//    nSamples = light->numSamples();
//     
//    for (size_t k=0; k<nSamples; k++)
//    {
//      sr.lightSamplePnt = light->getSamplePoints(sr)[k];
//
//      Vector wi = light->getDirection(sr);
//      double ndotwi = sr.normal * wi;
//
//      if (ndotwi > 0.0) 
//      {
//        bool inShadow = false;
//
//        if (light->castsShadow() )
//        {
//          Ray shadowRay(sr.hitPoint, wi);
//          inShadow = light->inShadow(shadowRay, sr);
//        }
//
//        if (!inShadow)
//          lightL += _diffuseBRDF->f(sr, wo, wi) * light->L(sr) *
//                    light->G(sr) * static_cast<float>(ndotwi) /
//                    light->pdf(sr);
//      }
//    }
//    lightL /= static_cast<float>(nSamples);
//    resL += lightL;
//  }
//
//  return resL;
//
//}


//_____________________________________________________________________________
Spectrum
Matte::areaLightShade(ShadeRec& sr) const
{
  Vector wo = -1 * sr.ray.d();
  Spectrum resL = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);
  size_t nLights = sr.world.lights.size();
  size_t   nSamples;
  Light* light;

  for (int j = 0; j < nLights; j++)
  {
    Spectrum lightL = BLACK;
    light = sr.world.lights[j];
    nSamples = light->numSamples();
     
    for (size_t k=0; k<nSamples; k++)
    {
      sr.lightSamplePnt = light->getSamplePoints(sr)[k];

      Vector wi = light->getDirection(sr);
      double ndotwi = sr.normal * wi;

      if (ndotwi > 0.0) 
      {
        bool inShadow = false;

        if (light->castsShadow() )
        {
          Ray shadowRay(sr.hitPoint, wi);
          inShadow = light->inShadow(shadowRay, sr);
        }

        if (!inShadow)
          lightL += _diffuseBRDF->f(sr, wi, wo) * light->L(sr) *
                    light->G(sr) * static_cast<float>(ndotwi) /
                    light->pdf(sr);
      }
    }
    lightL /= static_cast<float>(nSamples);
    resL += lightL;
  }

  return resL;
}


//_____________________________________________________________________________
Spectrum
Matte::globalShade(ShadeRec& sr) const
{
  Vector wi;
  Vector wo = -1 * sr.ray.d();
  float pdf;
  Spectrum resSpec = _diffuseBRDF->sampleF(sr, wi, wo, pdf);
  float ndotwi = static_cast<float>(sr.normal * wi);
  Ray reflectedRay(sr.hitPoint, wi);

  return resSpec * sr.world.globalRenderer->Li(reflectedRay, sr.depth + 1) *
         ndotwi / pdf;

}


//_____________________________________________________________________________
Spectrum
Matte::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  Vector wi = -1 * photonDir;
  Vector wo = -1 * sr.ray.d();
  float ndotwi = static_cast<float>(sr.normal * wi);


  return _diffuseBRDF->f(sr, wi, wo) * ndotwi;
}


//_____________________________________________________________________________
bool
Matte::tracePhoton(const TracePhoton& curPhoton, TracePhoton& tracedPhoton,
                   const ShadeRec& sr) const
{
 // Das nachfolgende ist eine Vereinfachung um die Berechnungne zu
  // beschleunigen und hält sich nicht an ein physikalisches Modell.
  // Dies ist nur möglich weil die einfallende Energie uniform über die 
  // Hemisphäre verteilt wird.
  float dummy;
  Vector wo;
  std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  Spectrum BRDFValue = 
      _diffuseBRDF->photonSample(sr, curPhoton.photonRay.d(), wo , dummy);
  float prob = RefProbability::getProb(BRDFValue);

  // Russisches Roulette benutzen um festzustellen ob Photon absorbiert
  // oder reflektiert wird.
  float res = MutexedRNG::number(uniformDist);


  if (res <= prob)
  {
    assert(gutils::equals(wo.length2(), 1. ) ); 
    tracedPhoton.color = curPhoton.color * BRDFValue;
    tracedPhoton.photonRay = Ray(sr.hitPoint, wo );
    tracedPhoton.onlySpecularRefl = false;

    assert(!_isnan(tracedPhoton.color[0]) && !_isnan(tracedPhoton.color[1]) &&
           !_isnan(tracedPhoton.color[2]) );

    return false;
  }
  else
  {
    tracedPhoton = curPhoton;
    tracedPhoton.photonRay = Ray(sr.hitPoint, sr.ray.d() );

    return true;
  }


  return true;
}