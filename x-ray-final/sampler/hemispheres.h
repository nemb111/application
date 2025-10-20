/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 129ff.
*/


#pragma once

#ifndef HEMISPHERES_H_
#define HEMISPHERES_H_

#include <vector>

#include "linAlg.h"
#include "unitsampler.h"
// Liefert Punkte auf einer Hemisphäre gemäß einer angegebenen Kosinusverteilung
// der Form cos^e. Der Scheitelpunkt der Hemisphäre befindet sich dabei
// exakt auf der z-Achse des globalen Koordinatensystems.
class HemisphereS
{
public:
  HemisphereS(UnitSampler* unitSampler=nullptr, double e=0.);
  virtual ~HemisphereS();
 
  virtual HemisphereS* clone() const;
  virtual std::string className() const {return "HemisphereS";}
  uint createSamples(std::vector<Point>& samplePoints);
  uint getNumSamples() const;
  double getE() const {return _e;}
  const UnitSampler* getUnitSampler() const {return _unitSampler;}

protected:

private:
  double _e;
  UnitSampler* _unitSampler;
};

#endif //HEMISPHERES_H_