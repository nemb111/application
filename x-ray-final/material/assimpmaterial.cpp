
#include "stdafx.h"

//#include <random>

//#include "assimpmaterial.h"
//#include "mutexedrng.h"
//#include "normalmap.h"
//#include "photonrenderer.h"
//#include "refprobability.h"


//_____________________________________________________________________________
AssimpMaterial::AssimpMaterial(const Spectrum& ambientS, 
                               const Spectrum& diffuseS,
                               const Spectrum& specularS, double specExp, 
                               const Texture* ambientT/*=NULL*/,
                               const Texture* diffuseT/*=NULL*/,
                               const Texture* specularT/*=NULL*/,
                               const NormalMap* normalMap/*NULL*/)
  : _ambientBRDF(new LambertianTex(ambientS, 1., ambientT) ),
    _diffuseBRDF(new LambertianTex(diffuseS, 1., diffuseT) ),
    _specularBRDF(new SpecularTex(specularS, 1., specExp, specularT) ),
    _normalMap(normalMap)
{
}


//_____________________________________________________________________________
AssimpMaterial::~AssimpMaterial(void)
{
  delete _ambientBRDF;
  _ambientBRDF = 0;
  delete _diffuseBRDF;
  _diffuseBRDF = 0;
  delete _specularBRDF;
  _specularBRDF = 0;
}


//_____________________________________________________________________________
//Spectrum 
//AssimpMaterial::shade(ShadeRec& sr) const
//{
//  Vector wo(-1 * sr.ray.d() );
//  Spectrum L = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);
//
//  //Ersetze die Normale gemäß der Normalen map
//  if (_normalMap != NULL)
//    sr.normal = _normalMap->getNormal(sr);
//
//
//  size_t nLights = sr.world.lights.size();
//  for (size_t i=0; i<nLights; i++)
//  {
//    Vector wi(sr.world.lights[i]->getDirection(sr) );
//    float cos = static_cast<float>(sr.normal * wi);
//
//    if (cos > 0.0)
//    {
//      bool inShadow = false;
//      if(sr.world.lights[i]->castsShadow() )
//      {
//        Ray shadowRay(sr.hitPoint, wi);
//        inShadow |= sr.world.lights[i]->inShadow(shadowRay, sr);
//      }
//      
//      if (!inShadow)
//      L += (_diffuseBRDF->f(sr, wo, wi) + _specularBRDF->f(sr, wo, wi) )
//           * sr.world.lights[i]->L(sr) * cos;
//
//    }
//  }
//
//  return L;
//}


//_____________________________________________________________________________
Spectrum
AssimpMaterial::areaLightShade(ShadeRec& sr) const
{
  size_t nSamples;
  Light* light;
  size_t nLights = sr.world.lights.size();
  Vector wo = -1 * sr.ray.d();
  Spectrum resL = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);

  //Ersetze die Normale gemäß der Normalen map
  if (_normalMap != NULL)
    sr.normal = _normalMap->getNormal(sr);

  for (int j = 0; j < nLights; j++)
  {
    Spectrum lightL = BLACK;
    light = sr.world.lights[j];
    nSamples = light->numSamples();
    const std::vector<const Point>& samplePoints = light->getSamplePoints(sr);
     
    for (size_t k=0; k<nSamples; k++)
    {
      //sr.lightSamplePnt = light->getSamplePoints(sr)[k];
      sr.lightSamplePnt = samplePoints[k];

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
        {
          //Führe die Rendering Equation durch
          lightL += (_diffuseBRDF->f(sr, wi, wo) + _specularBRDF->f(sr, wi, wo) )
                    * light->L(sr) * light->G(sr) * static_cast<float>(ndotwi) /
                    light->pdf(sr);
        }
      }
    }
    lightL /= static_cast<float>(nSamples);
    resL += lightL;
  }

  return resL;
}


//_____________________________________________________________________________
Spectrum
AssimpMaterial::globalShade(ShadeRec& sr) const
{
  //TODO: Bisher nur der diffus teil bei globaler Beleuchtung berücksichtig
  //      Phong Teil fehlt noch
  Vector wi;
  Vector wo = -1 * sr.ray.d();
  float pdf;
  Spectrum spec;
  Spectrum resSpec = BLACK;


  for (int i=0; i<1; i++)
  {
    spec = _diffuseBRDF->sampleF(sr, wi, wo, pdf);
    float ndotwi = static_cast<float>(sr.normal * wi);
    Ray reflectedRay(sr.hitPoint, wi);

    resSpec += spec * sr.world.globalRenderer->Li(reflectedRay, sr.depth + 1, sr.contribution) *
                ndotwi / pdf;
  }


  return resSpec * 1.7f;
}


//_____________________________________________________________________________
Spectrum
AssimpMaterial::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  Vector wi = -1 * photonDir;
  Vector wo = -1 * sr.ray.d();
  float ndotwi = static_cast<float>(sr.normal * wi);

  if (ndotwi < 0)
    return BLACK;
  else
    return (_diffuseBRDF->f(sr, wi, wo) + _specularBRDF->f(sr, wi, wo) ) * ndotwi;
}


//_____________________________________________________________________________
bool
AssimpMaterial::tracePhoton(const TracePhoton& curPhoton, 
                            TracePhoton& tracedPhoton,
                            const ShadeRec& sr) const
{
  //// Das nachfolgende ist eine Vereinfachung um die Berechnungne zu
  //// beschleunigen und hält sich nicht an ein physikalisches Modell.
  //// Dies ist nur möglich weil die einfallende Energie uniform über die 
  //// Hemisphäre verteilt wird.
  //float dummy;
  //Vector wo;
  //std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  //Spectrum BRDFValue = 
  //    _diffuseBRDF->photonSample(sr, curPhoton.photonRay.d(), wo, dummy);
  //float prob = RefProbability::getProb(BRDFValue);

  //// Russisches Roulette benutzen um festzustellen ob Photon absorbiert
  //// oder reflektiert wird.
  //float res = MutexedRNG::number(uniformDist);


  //if (res <= prob)
  //{
  //  assert(gutils::equals(wo.length2(), 1. ) ); 
  //  tracedPhoton.color = curPhoton.color * BRDFValue;
  //  tracedPhoton.photonRay = Ray(sr.hitPoint, wo );
  //  tracedPhoton.onlySpecularRefl = false;
  //  

  //  assert(!_isnan(tracedPhoton.color[0]) && !_isnan(tracedPhoton.color[1]) &&
  //         !_isnan(tracedPhoton.color[2]) );

  //  return false;
  //}
  //else
  //{
  //  tracedPhoton = curPhoton;
  //  tracedPhoton.photonRay = Ray(sr.hitPoint, sr.ray.d() );

  //  return true;
  //}

  // Das Photon trifft auf der Rückseite auf
   //Ersetze die Normale gemäß der Normalen map
  if (_normalMap != NULL)
    const_cast<ShadeRec&>(sr).normal = _normalMap->getNormal(sr);


  if (sr.normal * curPhoton.photonRay.d() > 0)
  {
    tracedPhoton.color = BLACK;
    tracedPhoton.intensity = 0;

    return false;
  }

  float dummy;
  Vector wi = -1 * curPhoton.photonRay.d();
  Vector wo;
  std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  Spectrum diffRefL = _diffuseBRDF->photonSample(sr, wi, wo , dummy);
  Spectrum specRefL = _specularBRDF->f(sr, wi, wo);
  float diffRefLProb = RefProbability::getProb(diffRefL);
  float specRefLProb = RefProbability::getProb(specRefL);

  // Russisches Roulette benutzen um festzustellen ob Photon absorbiert
  // oder reflektiert wird.
  float res = MutexedRNG::number(uniformDist);


  if (res <= diffRefLProb)
  {
    assert(gutils::equals(wo.length2(), 1. ) ); 
    tracedPhoton.color = curPhoton.color * diffRefL;
    tracedPhoton.photonRay = Ray(sr.hitPoint, wo );
    tracedPhoton.onlySpecularRefl = false;

    assert(!_isnan(tracedPhoton.color[0]) && !_isnan(tracedPhoton.color[1]) &&
           !_isnan(tracedPhoton.color[2]) );

    return false;
  }
  else if (res <= diffRefLProb + specRefLProb)
  {
    tracedPhoton.color = curPhoton.color * diffRefL * specRefL / specRefLProb;
    tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
    tracedPhoton.onlySpecularRefl = false;

    return false;
  }
  else
  {
    tracedPhoton = curPhoton;
    tracedPhoton.photonRay = Ray(sr.hitPoint, sr.ray.d() );

    return true;
  }


  return false;
}