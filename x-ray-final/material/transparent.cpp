#include "stdafx.h"

//#include "perfectspecular.h"
//#include "perfecttransmitter.h"
//#include "transparent.h"


//_____________________________________________________________________________
Transparent::Transparent(Spectrum objC, Spectrum specC, double ambientRefl,
                         double diffuseRefl, double specularRefl, double exp
                         /*Phong*/, Spectrum perfSpecC, double perfSpecRefl
                         /*PerfectSpecular*/, float transCoeff,
                         float idxOfRefraction/*PerfectTransmitter*/ )
  : Phong(objC, specC, ambientRefl, diffuseRefl, specularRefl, exp),
    _pSpecularBRDF(new PerfectSpecular(perfSpecC, perfSpecRefl) ),
    _pTransmitterBTDF(new PerfectTransmitter(transCoeff, idxOfRefraction) )
{
}


//_____________________________________________________________________________
Transparent::~Transparent(void)
{
  delete _pSpecularBRDF;
  _pSpecularBRDF = 0;
  delete _pTransmitterBTDF;
  _pTransmitterBTDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//Transparent::shade(ShadeRec& sr) const
//{
//  Spectrum L = Phong::shade(sr);
//
//  Vector wo = -1 * sr.ray.d();
//  Vector wi;
//  Spectrum fr = _pSpecularBRDF->sampleF(sr, wo, wi); // berechnet wi
//  Ray reflectedRay(sr.hitPoint, wi);
//
//  if (_pTransmitterBTDF->tir(sr, wo) )
//    L += sr.world.renderer->Li(reflectedRay, sr.depth + 1); // kr = 1.0
//  else
//  {
//    Vector wt;
//    Spectrum ft = _pTransmitterBTDF->sampleF(sr, wo, wt); // berechnet wt
//    Ray transmittedRay(sr.hitPoint, wt);
//
//    L += fr * sr.world.renderer->Li(reflectedRay, sr.depth + 1) *
//         static_cast<float>(fabs(sr.normal * wi) );
//    L += ft * sr.world.renderer->Li(transmittedRay, sr.depth + 1) *
//         static_cast<float>(fabs(sr.normal * wt) );
//  }
//
//
//  return L;
//}


//_____________________________________________________________________________
Spectrum
Transparent::areaLightShade(ShadeRec& sr) const
{
  Spectrum L = Phong::areaLightShade(sr);
  float dummy;

  const Vector& wo = -1. * sr.ray.d();
  Vector wi;
  Spectrum fr = _pSpecularBRDF->sampleF(sr, wi, wo, dummy); // berechnet wi
  Ray reflectedRay(sr.hitPoint, wi);


  if (_pTransmitterBTDF->tir(sr, wo) )
  {
    L += sr.world.renderer->Li(reflectedRay, sr.depth + 1); // kr = 1.0
    L += sr.world.globalRenderer->Li(reflectedRay, sr.depth + 1);
  }
  else
  {
    Vector wt;
    Spectrum ft = _pTransmitterBTDF->sampleF(sr, wo, wt); // berechnet wt
    Ray transmittedRay(sr.hitPoint, wt);

  
    L += fr * (sr.world.renderer->Li(reflectedRay, sr.depth + 1,
                                     sr.contribution * fr.ColorFrac() *
                                     static_cast<float>(fabs(sr.normal * wi) )
                                    ) +
               sr.world.globalRenderer->Li(reflectedRay, 0) 
              ) *
         static_cast<float>(fabs(sr.normal * wi) );
    L += ft * (sr.world.renderer->Li(transmittedRay, sr.depth + 1,
                                     sr.contribution * ft.ColorFrac() *
                                     static_cast<float>(fabs(sr.normal * wt) )
                                    ) +
               sr.world.globalRenderer->Li(transmittedRay, 0)                     
              ) *
         static_cast<float>(fabs(sr.normal * wt) );
  }


  return L;
}


//_____________________________________________________________________________
Spectrum
Transparent::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{



  return BLACK;
}


//_____________________________________________________________________________
bool
Transparent::tracePhoton(const TracePhoton& curPhoton, 
                         TracePhoton& tracedPhoton,
                         const ShadeRec& sr) const
{
  // TODO: Es muss noch spekulare und diffuse brdf beachtet werden

  std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  float dummy;
  float probPSpecRefL;
  float randProb = MutexedRNG::number(uniformDist);
  Vector wo;
  Vector wt;
  Vector wi = -1 * curPhoton.photonRay.d();
  Spectrum pSpecRefL;
  Spectrum pTransRefL;


  pSpecRefL = _pSpecularBRDF->photonSample(sr, wi, wo, dummy);
  probPSpecRefL = RefProbability::getProb(pSpecRefL);


  //Zuerst wird überprüft ob sich das Photon im Shape befindet. Dann ist 
  // Absorption keine Option
  if (wi * sr.normal < 0)
  {
   
    // Es handelt sich um totale innere Reflexion also gibt es nur eine
    // Möglichkeit
    if (_pTransmitterBTDF->tir(sr, wi) )
    {
      tracedPhoton.color = curPhoton.color;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }

    // Also keine innere Reflexion, jetzt muss gewürfelt werden
    if (randProb <= probPSpecRefL)
    {
      tracedPhoton.color = curPhoton.color * pSpecRefL / probPSpecRefL;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }
    else
    {
      pTransRefL = _pTransmitterBTDF->photonSample(sr, wi, wt, dummy);

      tracedPhoton.color = curPhoton.color;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wt);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }

  }
  // Das Photon trifft von außen auf das Shape
  else
  {

    // Es handelt sich um totale äußere Reflexion also gibt es nur eine
    // Möglichkeit
    if (_pTransmitterBTDF->tir(sr, wi) )
    {
      tracedPhoton.color = curPhoton.color;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }

    if (randProb <= probPSpecRefL)
    {
      tracedPhoton.color = curPhoton.color * pSpecRefL / probPSpecRefL;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wo);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }
    else
    {
      pTransRefL = _pTransmitterBTDF->photonSample(sr, wi, wt, dummy);

      tracedPhoton.color = curPhoton.color;
      tracedPhoton.photonRay = Ray(sr.hitPoint, wt);
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }

  }


  return false;
}