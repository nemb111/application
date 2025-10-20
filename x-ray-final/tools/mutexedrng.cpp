#include "stdafx.h"

//#include "mutexedrng.h"


//_____________________________________________________________________________
std::mt19937 MutexedRNG::_rng;


//_____________________________________________________________________________
HANDLE MutexedRNG::_mutex(CreateMutex(NULL, FALSE, NULL) );


//_____________________________________________________________________________
MutexedRNG::MutexedRNG(void)
{
}


MutexedRNG::~MutexedRNG(void)
{
}
