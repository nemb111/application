#pragma once

#ifndef LIGHTSHAPE_H_
#define LIGHTSHAPE_H_

#include "shape.h"
#include "unitsampler.h"

class LightShape : public Shape
{
public:
  LightShape(void);
  LightShape(float invArea);
  virtual ~LightShape(void);

  virtual Point getSample() = 0;
  virtual float pdf(const ShadeRec& sr) const = 0;
  virtual Normal getNormal(const ShadeRec& sr) const = 0;
  virtual float getArea() const = 0;
  virtual void setUnitSampler(UnitSampler* uSampler);
  virtual void resetSampler();
  virtual uint numSamples() const;
  virtual uint numSamplesCreated() const {return _uSampler->numSamplesCreated();}
  virtual uint getId() const {return _id;}

protected:
  static uint id;
  uint _id;
  float _invArea;
  UnitSampler* _uSampler;
  

};

#endif //LIGHTSHAPE_H_