#pragma once

#ifndef BV_H_
#define BV_H_

#include "ray.h"

//Forward Deklarationen
class Ray;

class BV
{
public:
  BV();
  virtual ~BV();

  //static BV empty()              {EOUT("Methode nicht implementiert."); exit(1);}

  virtual bool hit(const Ray& ray) const = 0;

  //void extend(const Point& point){EOUT("Methode nicht implementiert."); exit(1);}
  //void extend(const BV& bv)      {EOUT("Methode nicht implementiert."); exit(1);}
  //float getArea() const          {EOUT("Methode nicht implementiert."); exit(1);}
  //Vector diagonal() const        {EOUT("Methode nicht implementiert."); exit(1);}




protected:

private:
};

#endif//BV_H_