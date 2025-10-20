#include "stdafx.h"

//#include "speculartex.h"


//_____________________________________________________________________________
SpecularTex::SpecularTex(void)
  : Specular()
{
}


//_____________________________________________________________________________
SpecularTex::SpecularTex(const Spectrum& spectrum, double reflectance/*=1.0*/, 
                         double exp/*=100*/, const Texture* texture/*=NULL*/)
  : Specular(spectrum, reflectance, exp), _cst(texture)
{
}


//_____________________________________________________________________________
SpecularTex::~SpecularTex(void)
{
}


//_____________________________________________________________________________
Spectrum 
SpecularTex::f(const ShadeRec& shadeRec, const Vector& wi,
               const Vector& wo) const
{
// Verwende Alphablending um die Farbe der Spekularen Reflexion mit der Textur
// zu vermischen
  if (_cst != NULL)
  {
    double ndotwi = shadeRec.normal * wi;
    Vector r(-1 * wi + 2.0 * shadeRec.normal * ndotwi);
    double rcos = r * wo;

    if (rcos < 0.0)
      return BLACK;

    const RGBASpectrum rgba = _cst->getSpectrum(shadeRec);
    float alpha = rgba.getAlpha();
    Spectrum cst = rgba.getRGB();

    return ((alpha*cst) + (1-alpha)*_cs) * 
           static_cast<float>(_ks * pow(rcos, _exp) );


  }
  else
    return Specular::f(shadeRec, wi, wo);

}


//_____________________________________________________________________________
Spectrum 
SpecularTex::sampleF(const ShadeRec& shadeRec, Vector& wi, const Vector& wo,
                     float& pdf) const
{
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
SpecularTex::rho(const ShadeRec& shadeRec, const Vector& wo) const
{
  return BLACK;
}
