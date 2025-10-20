#pragma once

#ifndef REFPROBABILITY_H_
#define REFPROBABILITY_H_


#include "spectrum.h"


namespace REFPROBABILITY
{
  enum Modus : char
  {
    TOTALENERGY = 0,
    MAXENERGY = 1
  };


}

class RefProbability
{
public:

  typedef REFPROBABILITY::Modus Modus;
  
  static void setModus(const Modus& modus);

  static float getProb(const Spectrum& spectrum);


private:
  
  RefProbability(void);
  ~RefProbability(void);

  static Modus _modus;

};


#endif //REFPROBABILITY_H_