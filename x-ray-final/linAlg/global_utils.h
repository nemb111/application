#ifndef GLOBAL_UTILS_H_
#define GLOBAL_UTILS_H_

#define NOMINMAX
#include <Windows.h>
#include <iostream>

#include "linAlg.h"

namespace gutils
{

#if PRECISION == 1
const FLOATING EPSILON = (FLOATING)1E-5;
#else
const double EPSILON = 1E-14;
#endif

  //using std::cerr;
  //using std::endl;

/******************Makro-definitions********************/
#ifdef _DEBUG
#define gutils_assert(x) if((!(x) ) ) \
          { gutils_EOUT("Assertion fail"); DebugBreak();}
#else
#define gutils_assert(x)
#endif//_DEBUG

#define gutils_IOUT(x) std::cout << "INFO: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";

#define gutils_WOUT(x) std::cerr << "WARNING: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";
                  
#define gutils_EOUT(x) std::cerr << "ERROR: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";



template<class T>
inline bool equals(T first, T second)
{
  return abs(first - second) < EPSILON;
}







} // namespace gutils

#endif //GLOBAL_UTILS_H_