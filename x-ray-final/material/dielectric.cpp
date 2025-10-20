#include "stdafx.h"

//#include "dielectric.h"
//#include "fresnelreflector.h"
//#include "fresneltransmitter.h"

//_____________________________________________________________________________
//Dielectric::Dielectric(void)
//{
//}


//_____________________________________________________________________________
Dielectric::Dielectric(const Spectrum& objC, const Spectrum& specC,
                       double ambientRefl, double diffuseRefl, 
                       double specularRefl, double exp, float etaOuterior,
                       float etaInterior, const Spectrum& inColorFilter,
                       const Spectrum& outColorFilter, float dOutCoeff/*=1.f*/,
                       float dInCoeff/*=1.f*/)
  : Phong(objC, specC, ambientRefl, diffuseRefl, specularRefl, exp),
    _cfIn(inColorFilter), _cfOut(outColorFilter),
    _fresnelRefBRDF(new FresnelReflector(etaInterior, etaOuterior) ),
    _fresnelTransBTDF(new FresnelTransmitter(etaInterior, etaOuterior) ),
    _dOutCoeff(dOutCoeff), _dInCoeff(dInCoeff)
{
  assert(inColorFilter[0] >= 0.f && inColorFilter[0] <= 1.f);
  assert(inColorFilter[1] >= 0.f && inColorFilter[1] <= 1.f);
  assert(inColorFilter[2] >= 0.f && inColorFilter[2] <= 1.f);
  assert(outColorFilter[0] >= 0.f && outColorFilter[0] <= 1.f);
  assert(outColorFilter[1] >= 0.f && outColorFilter[1] <= 1.f);
  assert(outColorFilter[2] >= 0.f && outColorFilter[2] <= 1.f);

}


//_____________________________________________________________________________
Dielectric::~Dielectric(void)
{
  delete _fresnelRefBRDF;
  _fresnelRefBRDF = 0;
  delete _fresnelTransBTDF;
  _fresnelTransBTDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//Dielectric::shade(ShadeRec& sr) const
//{
//  Spectrum L(Phong::shade(sr) );
//
//  Vector wi;
//  Vector wo = -1. * sr.ray.d();
//  Spectrum fr = _fresnelRefBRDF->sampleF(sr, wi, wo); // computes wi
//  Ray reflectedRay(sr.hitPoint, wi);
//  //float t;
//  Spectrum Lr, Lt;
//  float ndotwi = static_cast<float>(sr.normal * wi);
//
//  if (_fresnelTransBTDF->tir(sr, wo) )  // total internal reflection
//  {
//    if (ndotwi < 0.0f) // reflected ray is inside
//    {
//      Lr = sr.world.renderer->Li(reflectedRay, sr.depth + 1);
//      // In sr.t befindet sich jetzt tmin des reflektierten Ray, falls dieser
//      // ein Objekt trifft
//      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lr;
//    }
//    else 
//    {
//      // reflected ray is outside
//
//      Lr = sr.world.renderer->Li(reflectedRay, sr.depth + 1);
//      // kr = 1
//      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) * Lr;  // outside filter color
//    }
//  }
//  else // no total internal reflection
//  {
//    Vector wt;
//    Spectrum ft = _fresnelTransBTDF->sampleF(sr, wo, wt);
//    Ray transmittedRay(sr.hitPoint, wt);
//    float ndotwt = static_cast<float>(sr.normal * wt);
//
//    if (ndotwi < 0.0f) // reflected ray is inside
//    {
//      Lr = fr * sr.world.renderer->Li(reflectedRay, sr.depth + 1) * 
//           fabs(ndotwi);
//      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lr; // inside filter color
//
//      // transmitted ray is outside
//
//      Lt = ft * sr.world.renderer->Li(transmittedRay, sr.depth + 1) * 
//           fabs(ndotwt);
//      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) * Lt; // outside filter color
//    }
//    else // reflected ray is outside
//    {
//      Lr = fr * sr.world.renderer->Li(reflectedRay, sr.depth + 1) *
//           fabs(ndotwi);
//      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) * Lr; // outside filter color
//
//      // transmitted ray is inside
//
//      Lt = ft * sr.world.renderer->Li(transmittedRay, sr.depth + 1) *
//           fabs(ndotwt);
//      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lt; // inside filter color
//
//    }
//  }
//
//  return L;
//}


//_____________________________________________________________________________
Spectrum
Dielectric::areaLightShade(ShadeRec& sr) const
{
 Spectrum L(Phong::areaLightShade(sr) );
 float dummy;

  Vector wi;
  Vector wo = -1. * sr.ray.d();
  Spectrum fr = _fresnelRefBRDF->sampleF(sr, wi, wo, dummy); // computes wi
  float kr = _fresnelRefBRDF->fresnel(sr);
  fr = kr * fr; // computes radiance according to
                // fresnel equation
  Ray reflectedRay(sr.hitPoint, wi);
  //float t;
  Spectrum Lr, Lt;
  float ndotwi = static_cast<float>(sr.normal * wi);

  if (_fresnelTransBTDF->tir(sr, wo) )  // total reflection
  {
    if (ndotwi < 0.0f) // reflected ray is inside
    {
      Lr = (sr.world.renderer->Li(reflectedRay, sr.depth + 1, sr.contribution) );
      // In sr.t befindet sich jetzt tmin des reflektierten Ray, falls dieser
      // ein Objekt trifft
      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lr;
    }
    else 
    {
      // reflected ray is outside

      Lr = (sr.world.renderer->Li(reflectedRay, sr.depth + 1, sr.contribution) +
            sr.world.globalRenderer->Li(reflectedRay, 0) );
      // kr = 1
      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff) * Lr;  // outside filter color
    }
  }
  else // no total reflection
  {
    Vector wt;
    Spectrum ft = _fresnelTransBTDF->sampleF(sr, wo, wt);
    float kt = _fresnelTransBTDF->fresnel(sr);
    ft = kt * ft; // computes radiance according to
                                              // fresnel equation.
    Ray transmittedRay(sr.hitPoint, wt);
    float ndotwt = static_cast<float>(sr.normal * wt);

    if (ndotwi < 0.0f) // reflected ray is inside
    {
      Lr = fr * (sr.world.renderer->Li(reflectedRay, sr.depth + 1,
                                       sr.contribution * kr ) ) * 
           fabs(ndotwi);
      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lr; // inside filter color

      // transmitted ray is outside

      Lt = ft * (sr.world.renderer->Li(transmittedRay, sr.depth + 1,
                                       sr.contribution * kt) +
                 sr.world.globalRenderer->Li(transmittedRay, 0)
                ) * 
           fabs(ndotwt);
      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) * Lt; // outside filter color
    }
    else // reflected ray is outside
    {
      Lr = fr * (sr.world.renderer->Li(reflectedRay, sr.depth + 1,
                                       sr.contribution * kr ) +
                 sr.world.globalRenderer->Li(reflectedRay, 0) 
                ) *
           fabs(ndotwi);
      L += Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) * Lr; // outside filter color

      // transmitted ray is inside

      Lt = ft * (sr.world.renderer->Li(transmittedRay, sr.depth + 1,
                                       sr.contribution * kt) ) *
           fabs(ndotwt);
      L += Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) * Lt; // inside filter color

    }
  }

  return L;
}


//_____________________________________________________________________________
Spectrum
Dielectric::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{


  return BLACK;
}


//_____________________________________________________________________________
bool
Dielectric::tracePhoton(const TracePhoton& curPhoton, 
                        TracePhoton& tracedPhoton,
                        const ShadeRec& sr) const
{
  std::uniform_real_distribution<float> uniformDist(0.0f, 1.0f);
  float dummy;
  float probFSpecRefL;
  float randProb = MutexedRNG::number(uniformDist);
  Vector wo;
  //Vector wt;
  Vector wi = -1 * curPhoton.photonRay.d();
  Spectrum fresSpecRefL;


  fresSpecRefL = _fresnelRefBRDF->photonSample(sr, wi, wo, dummy);
  probFSpecRefL = RefProbability::getProb(fresSpecRefL);

  Ray reflectedRay(sr.hitPoint, wo);

  float ndotwo = static_cast<float>(sr.normal * wo);


  // Pr¸fe auf totale Reflexion
  if (_fresnelTransBTDF->tir(sr, wi) )
  {
    if (ndotwo < 0.0f) // aktuelles Photon befindet sich im Shape
    {
      tracedPhoton.color = Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) *
                           curPhoton.color;
      tracedPhoton.photonRay = reflectedRay;
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }
    else // aktuelles Photon befindet sich auﬂerhalb des Shape
    {
      tracedPhoton.color = Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff) *
                           curPhoton.color;
      tracedPhoton.photonRay = reflectedRay;
      tracedPhoton.onlySpecularRefl &= true;

      return false;
    }
  }
  else // keine totale Reflexion
  {
    Vector wt;
    Spectrum fresTransRefL = _fresnelTransBTDF->photonSample(sr, wi, wt, dummy);
    Ray transmittedRay(sr.hitPoint, wt);
    float probFTransRefL = RefProbability::getProb(fresTransRefL);

    if (ndotwo < 0.0f) // aktuelles Photon im Inneren; reflektiertes Photon auch im inneren
    {
      if (randProb <= probFSpecRefL)
      {
        tracedPhoton.color = Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) *
                           curPhoton.color;
        tracedPhoton.photonRay = reflectedRay;
        tracedPhoton.onlySpecularRefl &= true;

        return false;
      }
      else // Durchlaufenes Photon wird auﬂerhalb sein
      {
        tracedPhoton.color = Pow(_cfIn, static_cast<float>(sr.t) * _dInCoeff ) *
                             curPhoton.color;
        tracedPhoton.photonRay = transmittedRay;
        tracedPhoton.onlySpecularRefl &= true;

        return false;
      }

    }
    else // aktuelles Photon auﬂerhalb; reflektiertes Photon auch auﬂerhalb
    {
      if (randProb <= probFSpecRefL)
      {
        tracedPhoton.color = Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) *
                             curPhoton.color;
        tracedPhoton.photonRay = reflectedRay;
        tracedPhoton.onlySpecularRefl &= true;

        return false;
      }
      else // Durchlaufenes Photon wird im Inneren sein
      {
        tracedPhoton.color = Pow(_cfOut, static_cast<float>(sr.t) * _dOutCoeff ) *
                             curPhoton.color;
        tracedPhoton.photonRay = transmittedRay;
        tracedPhoton.onlySpecularRefl &= true;

        return false;
      }
    }
  }


}