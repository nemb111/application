#include "stdafx.h"

//#include "hemispheres.h"
//#include "realisticmirror.h"
//#include "samplestorage.h"
//#include "stratifieds.h"


//_____________________________________________________________________________
RealisticMirror::RealisticMirror(void)
  : Phong(), _createdSampler(true), _exp(1.),
    _gSpecularBRDF(new GlossySpecular), _unitSampler(new StratifiedS(9) ),
    _hsphere(new HemisphereS(_unitSampler) )
{
  _gSpecularBRDF->setHemisphereS(_hsphere);
}


//_____________________________________________________________________________
RealisticMirror::RealisticMirror(Spectrum objC, Spectrum specC,
                                 Spectrum glossySpecC, double ambientRefl,
                                 double diffuseRefl, double specularRefl,
                                 double exp, double glossySpecularRefl,
                                 UnitSampler* unitSampler)
 : Phong(objC, specC, ambientRefl, diffuseRefl, specularRefl, exp), 
   _createdSampler(false), _exp(exp),
   _gSpecularBRDF(new GlossySpecular(glossySpecC, glossySpecularRefl, exp) ),
   _unitSampler(unitSampler), _hsphere(new HemisphereS(_unitSampler, _exp) )
{
  assert(_unitSampler);

  _gSpecularBRDF->setHemisphereS(_hsphere);
}


//_____________________________________________________________________________
RealisticMirror::~RealisticMirror(void)
{
  if (_createdSampler)
  {
    delete _unitSampler;
    _unitSampler = 0;
  }

  delete _hsphere;
  _hsphere = 0;
  delete _gSpecularBRDF;
  _gSpecularBRDF = 0;
}


//_____________________________________________________________________________
//Spectrum
//RealisticMirror::shade(ShadeRec& sr) const
//{
//  Spectrum dirL(Phong::areaLightShade(sr) );  //direct illumination
//
//  Vector u(-1.8473, 0.3183, 2.94729), v;
//  Vector wi;
//  Vector wo = -1. * sr.ray.d();
//  double ndotwo = wo * sr.normal;
//  // w = Richtung der perfekten Reflektion
//  Vector w = -1. * wo + 2.0 * sr.normal * ndotwo;
//  orthNormBasis(w, u, v);
//
//  Spectrum refL = BLACK;
//  const std::vector<const Point>* samplePoints =
//    SStorage::getHSphSamples(_hsphere);
//  size_t nSamples = samplePoints->size();
//  for (size_t i=0; i<nSamples; i++)
//  {
//    wi = u*(*samplePoints)[i].x + v*(*samplePoints)[i].y + 
//         w*(*samplePoints)[i].z;
//
//    if (sr.normal * wi < 0.0)
//      wi = u*-(*samplePoints)[i].x + v*-(*samplePoints)[i].y + 
//           w*(*samplePoints)[i].z;
//
//    Ray refRay = Ray(sr.hitPoint, wi);
//    refL += _gSpecularBRDF->sampleF(sr, wi, wo) *
//            sr.world.renderer->Li(refRay, sr.depth + 1) *
//            static_cast<float>(sr.normal * wi);
//  }
//  refL /= static_cast<float>(nSamples);
//
//  
//  return dirL + refL;
//}


//_____________________________________________________________________________
Spectrum
RealisticMirror::areaLightShade(ShadeRec& sr) const
{
  Spectrum dirL(Phong::areaLightShade(sr) );  //direct illumination
  float pdf = 0;

  Vector u(-1.8473, 0.3183, 2.94729), v;
  Vector wi;
  Vector wo = -1. * sr.ray.d();
  double ndotwo = wo * sr.normal;

  // Hier muss geprüft werden ob ndotwo negativ ist. Werden Strahlen ausgesandt,
  // die fast parallel zum Shape verlaufen kann es zum Selbstschnitt kommen.
  // Das sorgt dafür, dass der Strahl innerhalb des Shapes gefangen bleibt.
  // Dies verursacht eine komplizierte Version einer Endlosschleife.
  if (ndotwo <= 0)
    return BLACK;


  // w = Richtung der perfekten Reflektion
  Vector w = -1. * wo + 2.0 * sr.normal * ndotwo;
  orthNormBasis(w, u, v);

  Spectrum refL = BLACK;
  const std::vector<const Point>* samplePoints =
    SStorage::getHSphSamples(_hsphere);
  size_t nSamples = samplePoints->size();
  for (size_t i=0; i<nSamples; i++)
  {
    wi = u*(*samplePoints)[i].x + v*(*samplePoints)[i].y + 
         w*(*samplePoints)[i].z;

    if (sr.normal * wi < 0.0)
      wi = u*-(*samplePoints)[i].x + v*-(*samplePoints)[i].y + 
           w*(*samplePoints)[i].z;

    Ray refRay = Ray(sr.hitPoint, wi);


    refL += _gSpecularBRDF->sampleF(sr, wi, wo, pdf) *
            (sr.world.renderer->Li(refRay, sr.depth + 1) +
             sr.world.globalRenderer->Li(refRay, sr.depth + 1)
            ) *
            static_cast<float>(sr.normal * wi) / pdf;
  }
  refL /= static_cast<float>(nSamples);

  
  return dirL + refL;
}


//_____________________________________________________________________________
Spectrum
RealisticMirror::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  // TODO:: Später soll hier einfach photonShade der Basisklasse Phong benutzt
  // werden.
  Vector dummy1, dummy2;


  return Phong::photonShade(sr, photonDir);
}


//_____________________________________________________________________________
bool
RealisticMirror::tracePhoton(const TracePhoton& curPhoton, 
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
  Spectrum gspecRefL = _gSpecularBRDF->photonSample(sr, wi, wo, dummy);
  float probDiffRefL = RefProbability::getProb(diffRefL);
  float probGSpecRefL = RefProbability::getProb(gspecRefL);
  float randProb = MutexedRNG::number(uniformDist);


  // Photon wird absorbiert
  if (randProb > probDiffRefL + probGSpecRefL)
  {
    tracedPhoton = curPhoton;
    tracedPhoton.photonRay = Ray(sr.hitPoint, curPhoton.photonRay.d() );
    tracedPhoton.onlySpecularRefl &= true;

    return true;
  }
  // Photon wird glossy reflektiert
  else if (randProb > probDiffRefL)
  {
    tracedPhoton.color = curPhoton.color * gspecRefL / probGSpecRefL;
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