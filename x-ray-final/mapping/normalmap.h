#pragma once

#ifndef NORMALMAP_H_
#define NORMALMAP_H_

// Forward Deklarationen
class Normal;
class Texture;
class ShadeRec;

class NormalMap
{
public:
  NormalMap(const Texture* normalTex);
  virtual ~NormalMap(void);

  Normal getNormal(const ShadeRec& sr) const;
  //Normal getNormal(float u, float v) const;


private:
  const Texture* _normalTex;
};


#endif //NORMALMAP_H_