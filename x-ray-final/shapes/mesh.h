/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 478ff.
*/

#pragma once

#ifndef MESH_H_
#define MESH_H_

#include <map>
#include <vector>

#include "linAlg.h"
#include "x-ray.h"

using std::map;
using std::vector;


enum NORMALTYPE
{
  CALCNORMAL,
  FILENORMAL
};


class Mesh
{
public:
  Mesh(void) : numVertices(0), numTriangles(0), normalType(CALCNORMAL) {}
  virtual ~Mesh(void){}

  NORMALTYPE checkNormalType() const
  {
    return normalType;
  }

  vector<vector<double> > vertices;       //Vektor mit den Objekt-Punkten
  //vector<size_t> indices;                 //Punkt Index
  vector<vector<size_t> > faces;          //Vektor mit Punkten für eine geg. Seite
  vector<Normal> normals;                 //Normale eines Punktes
  vector<vector<size_t> > vertexFaces;    //Die am Punkt angrenzenden Dreiecksseiten
  vector<map<size_t, size_t> > fileNormalsIdx;
                                          //Gibt für geg. Seiten-Idx(face) und
                                          //Punkt-Idx den Normalen-Idx zurück
  vector<float> u;                        //u texture-Koordinate für jeden Punkt
  vector<float> v;                        //v texture-Koordinate für jeden Punkt
  uint numVertices;
  uint numTriangles;
  NORMALTYPE normalType;

};


#endif //MESH_H_