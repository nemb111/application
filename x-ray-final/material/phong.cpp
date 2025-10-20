#include "stdafx.h"

//#include "phong.h"



//_____________________________________________________________________________
Phong::Phong(void)
  : _ambientBRDF(new Lambertian(WHITE, 0.05) ), 
    _diffuseBRDF(new Lambertian() ),
    _specularBRDF(new Specular() )
{
}


//_____________________________________________________________________________
Phong::Phong(Spectrum objC, Spectrum specC, double ambientRefl, 
             double diffuseRefl, double specularRefl, double exp)
 : _ambientBRDF(new Lambertian(objC, ambientRefl) ),
   _diffuseBRDF(new Lambertian(objC, diffuseRefl) ),
   _specularBRDF(new Specular(specC, specularRefl, exp) )
{
}


//_____________________________________________________________________________
Phong::~Phong(void)
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
//Phong::shade(ShadeRec& sr) const
//{
//  Vector wo(-1 * sr.ray.d() );
//  Spectrum L = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);
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
Phong::areaLightShade(ShadeRec& sr) const
{
  size_t nSamples;
  Light* light;
  size_t nLights = sr.world.lights.size();
  Vector wo = -1 * sr.ray.d();
  Spectrum resL = _ambientBRDF->rho(sr, wo) * sr.world.ambientLight->L(sr);


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
          lightL += (_diffuseBRDF->f(sr, wi, wo) + _specularBRDF->f(sr, wi, wo) )
                    * light->L(sr) * light->G(sr) * static_cast<float>(ndotwi) /
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
Phong::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  Vector wo = -1 * sr.ray.d();
  Vector wi = -1 * photonDir;
  Spectrum LDiff = _diffuseBRDF->f(sr, wi, wo);
  Spectrum LSpec = _specularBRDF->f(sr, wi, wo);


  return LDiff + LSpec;
}


//_____________________________________________________________________________
bool
Phong::tracePhoton(const TracePhoton& curPhoton, TracePhoton& tracedPhoton,
                   const ShadeRec& sr) const
{
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