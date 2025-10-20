#include "stdafx.h"


//#include <algorithm>

//#include "refprobability.h"
//#include "spectrum.h"

using namespace REFPROBABILITY;

//_____________________________________________________________________________
Modus RefProbability::_modus = MAXENERGY;


//_____________________________________________________________________________
void
RefProbability::setModus(const Modus& modus)
{
  _modus = modus;
}


//_____________________________________________________________________________
float
RefProbability::getProb(const Spectrum& spectrum)
{
  int numSamples = spectrum.numSamples;
  float sum=0;

  if (_modus == TOTALENERGY)
  {
    for (int i=0; i<numSamples; i++)
      sum += spectrum[i];


    return sum / numSamples;
  }
  else if (_modus == MAXENERGY)
  {
    for (int i=0; i<numSamples; i++)
      sum = std::max(sum, spectrum[i] );


    return sum;
  }
  else
  {
    EOUT("Der Modus " << static_cast<int>(_modus) << "ist nicht verfuegbar.");
    std::cin.get();
    exit(EXIT_FAILURE);

    return 0;
  }


  return 0;
}


//_____________________________________________________________________________
RefProbability::RefProbability(void)
{
}


//_____________________________________________________________________________
RefProbability::~RefProbability(void)
{
}
