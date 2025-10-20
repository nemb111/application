#include "stdafx.h"

//#include "debugmaterial.h"


//_____________________________________________________________________________
DebugMaterial::DebugMaterial(void)
  : _color(WHITE)
{
}


//_____________________________________________________________________________
DebugMaterial::DebugMaterial(const Spectrum& color)
  : _color(color)
{
}


//_____________________________________________________________________________
DebugMaterial::~DebugMaterial(void)
{
}


//_____________________________________________________________________________
//Spectrum
//DebugMaterial::shade(ShadeRec& sr) const
//{
//  return _color;
//}


//_____________________________________________________________________________
Spectrum
DebugMaterial::areaLightShade(ShadeRec& sr) const
{
  return _color;
}