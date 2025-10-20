#pragma once

#ifndef MESHPARSER_H_
#define MESHPARSER_H_

#include <vector>

#include "linAlg.h"
#include "meshtriangle.h"

//Foward Deklarationen
class Mesh;
class Point;


using std::vector;

class MeshParser
{
public:
  MeshParser(void);
  MeshParser(const stdstring& filename) : _filename(filename) {}
  virtual ~MeshParser(void);

  virtual MeshParser* clone() const = 0;
  virtual bool fill(Mesh& mesh) = 0;

protected:
  stdstring _filename;

};

#endif//MESHPARSER_H_