/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 269ff.
*/


#pragma once

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "ray.h"
#include "spectrum.h"

//Forward declarations
class ShadeRec;
struct TracePhoton;

class Material {	
public:
	
  Material(void);						
  //Material(const Material& material); 				
  virtual ~Material(void);
				
  //virtual Spectrum shade(ShadeRec& sr) const;
  virtual Spectrum areaLightShade(ShadeRec& sr) const;
  virtual Spectrum globalShade(ShadeRec& sr) const;
  virtual Spectrum photonShade(const ShadeRec& sr, const Vector& photonDir)
    const;
  // true wenn das Photon absorbiert wird, false ansonsten.
  virtual bool tracePhoton(const TracePhoton& curPhoton, 
                           TracePhoton& tracedPhoton,
                           const ShadeRec& sr) const;
		
protected:
	
  //Material& operator= (const Material& rhs);						
};


#endif//MATERIAL_H_