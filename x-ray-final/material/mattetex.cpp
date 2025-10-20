#include "stdafx.h"

//#include "mattetex.h"



//_____________________________________________________________________________
MatteTex::MatteTex(Spectrum spectrum, double ambientRefl, double diffuseRefl,
                   const Texture* texture)
  : _ambientBRDF(new LambertianTex(spectrum, ambientRefl, texture) ), 
    _diffuseBRDF(new LambertianTex(spectrum, diffuseRefl, texture) )
{
}


//_____________________________________________________________________________
MatteTex::MatteTex(const Spectrum& ambientS, const Texture* ambientT,
                   const Spectrum& diffuseS, const Texture* diffuseT)
  : _ambientBRDF(new LambertianTex(ambientS, 1., ambientT) ),
    _diffuseBRDF(new LambertianTex(diffuseS, 1., diffuseT) )
{
}


//_____________________________________________________________________________
MatteTex::~MatteTex(void)
{
  delete _ambientBRDF;
  _ambientBRDF = 0;
  delete _diffuseBRDF;
  _diffuseBRDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//MatteTex::shade(ShadeRec& sr) const
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
//}


//_____________________________________________________________________________
Spectrum
MatteTex::areaLightShade(ShadeRec& sr) const
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