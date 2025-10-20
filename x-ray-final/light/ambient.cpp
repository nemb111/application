#include "stdafx.h"

//#include "ambient.h"


//_____________________________________________________________________________
Ambient::Ambient(void)
  : Light(), _ls(1.0), _spectrum(WHITE)
{
}


//_____________________________________________________________________________
Ambient::~Ambient(void)
{
}


//_____________________________________________________________________________
Normal
Ambient::getDirection(const ShadeRec& shadeRec) const
{
  return Vector(0,0,0);
}


//_____________________________________________________________________________
Spectrum 
Ambient::L(const ShadeRec& shadeRec)
{
  return _ls * _spectrum;
}


//_____________________________________________________________________________
bool
Ambient::inShadow(const Ray& ray, const ShadeRec& sr) const
{
  return false;
}