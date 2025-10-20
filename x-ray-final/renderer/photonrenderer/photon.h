#pragma once

#ifndef PHOTON_H_
#define PHOTON_H_

#include <memory>

#include "ray.h"

// Photon struct, die benutzt wird um das Photon in die Szene zu schießen
struct TracePhoton
{
  Ray photonRay;       // position + direction (incident direction)
  Spectrum color;      // A RGB color value
  float intensity;     
  unsigned short depth;// Gibt die Anzahl der Reflektionen an
  bool onlySpecularRefl;   // Ist true wenn alle Reflexionen bis zu Absorbtion spekulare waren.
};


// Reduziertes Photon struct, das zum Speichern und für die spätere NN-Abfrage
// verwendet wird.
struct Photon
{
  Point location;
  Vector wi;
  Spectrum color;
};


typedef std::unique_ptr<const TracePhoton> UPCTPhoton;
typedef std::unique_ptr<TracePhoton> UPTPhoton;

typedef std::unique_ptr<const Photon> UPCPhoton;

#endif //PHOTON_H_