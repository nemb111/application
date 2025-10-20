#include "stdafx.h"

//#include "material.h"


//_____________________________________________________________________________
Material::Material(void)
{
}


//_____________________________________________________________________________
Material::~Material(void)
{
}


//_____________________________________________________________________________
//Spectrum
//Material::shade(ShadeRec& sr) const
//{
//  EOUT("Diese Methode muss von abgeleiteten Klassen definiert werden!");
//  return BLACK;
//}


//_____________________________________________________________________________
Spectrum
Material::areaLightShade(ShadeRec& sr) const
{
  EOUT("Diese Methode muss von abgeleiteten Klassen definiert werden!");
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
Material::globalShade(ShadeRec& sr) const
{
  EOUT("Diese Methode muss von abgeleiteten Klassen definiert werden!");
  return BLACK;
}


//_____________________________________________________________________________
Spectrum
Material::photonShade(const ShadeRec& sr, const Vector& photonDir) const
{
  EOUT("Diese Methode muss von abgeleiteten Klassen definiert werden!");
  return BLACK;
}


//_____________________________________________________________________________
bool
Material::tracePhoton(const TracePhoton& curPhoton, TracePhoton& tracedPhoton,
                      const ShadeRec& sr) const
{
  EOUT("Diese Methode muss von abgeleiteten Klassen definiert werden!");
  return false;
}