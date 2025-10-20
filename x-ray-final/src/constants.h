#pragma once

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "linAlg.h"


//Ein sehr kleiner Wert, der von floats gerade noch so repräsentiert werden kann
#if PRECISION == 1
const float DELTA = 0.0001f;
#else
const float DELTA = 0.00001f;

#endif

//Für den Vergleich von FLOATINGs
const FLOATING EPSILON = 1E-14;

//Mindestwert für t, damit der Schnitt zwischen Ray und Objekt gültig ist
#if PRECISION == 1
const FLOATING KEPSILON = 5E-3;
#else
const FLOATING KEPSILON = 1E-4;
#endif


#endif//CONSTANTS_H_