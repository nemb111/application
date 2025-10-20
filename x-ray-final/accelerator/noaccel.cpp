#include "stdafx.h"

//#include "noaccel.h"
//#include "parser.h"

//_____________________________________________________________________________
NoAccel::NoAccel(void)
  : Accelerator()
{
}


//_____________________________________________________________________________
NoAccel::NoAccel(const std::vector<Shape*>& shapes)
  : Accelerator(shapes)
{
}


//_____________________________________________________________________________
NoAccel::~NoAccel(void)
{
}


//_____________________________________________________________________________
Accelerator*
NoAccel::newInstance() const
{
  Accelerator* accel = new NoAccel;
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
bool
NoAccel::create()
{
  return true;
}


//_____________________________________________________________________________
Accelerator*
NoAccel::clone() const
{
  Accelerator* accel = new NoAccel(*this);
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
bool
NoAccel::hit(const Ray& ray, size_t& shpIdx, double* tmin/*=NULL*/) const
{
  double t;
  double tm = std::numeric_limits<double>::max();
  bool hit = false;
  // Dummy-Variablen
  World* world = NULL;
  ShadeRec shadeRec(*world);

  for (size_t i=0; i<_shapesRef->size(); i++)
  {
    if ((*_shapesRef)[i]->hit(ray, t, shadeRec) && (t < tm) )
    {
      tm = t;
      shpIdx = i;
      hit = true;
    }
      
  }

  if (tmin)
    *tmin = tm;

  return hit;
}


//_____________________________________________________________________________
bool
NoAccel::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  double t;
  bool hit = false;
  tmin = std::numeric_limits<double>::max();
  
  ShadeRec resultRec(sr.world);

  for (size_t i=0; i<_shapesRef->size(); i++)
  {
    if ((*_shapesRef)[i]->hit(ray, t, sr) && (t < tmin) )
    {
      tmin = t;
      hit = true;
      resultRec = sr;
    }
      
  }

  sr = resultRec;

  return hit;
}


//_____________________________________________________________________________
bool 
NoAccel::shadowHit(const Ray& ray, double& tmin) const
{
  bool hit = false;
  double t;
  tmin = std::numeric_limits<double>::max();

  for (size_t i=0; i<_shapesRef->size(); i++)
  {
    if ( (*_shapesRef)[i]->getThrowsShadow() && 
         (*_shapesRef)[i]->shadowHit(ray, t) && (t < tmin) &&  t > KEPSILON)
    {
      hit |= true;
      tmin = t;
    }

  }

  return hit;
}