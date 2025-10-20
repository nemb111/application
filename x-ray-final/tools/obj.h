#pragma once

#ifndef OBJ_H_
#define OBJ_H_

#include <string>

#include "meshparser.h"


class OBJ : public MeshParser
{
public:
  OBJ(void);
  OBJ(const stdstring& filename) : MeshParser(filename){}
  virtual ~OBJ(void);

  virtual MeshParser* clone() const;
  virtual bool fill(Mesh& mesh);

protected:
  bool exVertices(const char* line, Mesh& mesh) const;
  bool exFaces(const char* line, Mesh& mesh)
    const;
  bool exTextureCoord(const char* line, Mesh& mesh) const;
  bool exNormals(const char* line, Mesh& mesh);
  bool exComment(const char* line, Mesh& mesh) const;
  bool exGroup(const char* line, Mesh& mesh) const;

  void fillNormals(Mesh& mesh) const;

};

#endif//OBJ_H_