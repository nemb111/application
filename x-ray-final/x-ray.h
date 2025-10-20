#pragma once

#ifndef X_RAY_H_
#define X_RAY_H_

#define NOMINMAX
#include "tchar.h"
#include <Windows.h>
// Die beiden folgenden includes werden benötigt um unicode verwenden zu können
#include <fcntl.h>
#include <io.h>

#include <iostream>

#include "constants.h"

//Forward Declarations
class GUIcomm;
class Options;

/*****************Typdeklartionen***************************/
typedef unsigned int uint;



/******************Makro-definitions********************/
#ifdef _DEBUG
#define assert(x) if(!(x)){ EOUT("Assertion fail"); DebugBreak();}
#else
#define assert(x)
#endif//_DEBUG



#define IOUT(x) std::wcout << "INFO: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";

#define WOUT(x) std::wcerr << "WARNING: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";
                  
#define EOUT(x) std::wcerr << "ERROR: " << x << " [" << __LINE__ <<" / "\
						              << __FUNCTION__ << "]\n";



#ifndef _UNICODE
typedef char cchar;
typedef std::string stdstring;
#else
typedef wchar_t cchar;
typedef std::wstring stdstring;
#endif


// Global Inline Functions
template<class T>
inline bool equals(T first, T second)
{
  return abs(first - second) < EPSILON;
}

template<class T>
inline bool inInterval(T target, T min, T max)
{
  assert(min <= max);

  return target >= min && target <= max;
}

inline unsigned int RoundUpEven(unsigned int v)
{
  return (v % 2) == 0 ? v : v + 1;
}

//Rundet auf die nächstgößere Zahl auf, die eine Zweierpotenz ist
inline unsigned int RoundUpPow2(unsigned int v) 
{
    v--;
    v |= v >> 1;    v |= v >> 2;
    v |= v >> 4;    v |= v >> 8;
    v |= v >> 16;
    return v+1;
}

//Rundet auf die nächste Quadratzahl auf
inline uint roundUpSqrNum(uint v)
{
  return static_cast<uint>(
    pow(static_cast<double>(
      static_cast<uint>(sqrt(static_cast<double>(v-1) ) + 1) ), 2) );
}


//Gibt eine Zufallszahl aus dem Einheitsintervall [0, 1] zurück
double randUI();


/*****************Start-Funktion****************************/

// Die Optionen müssen nur einmal geparst werden. Dies kann extern geschehen
// z.B. durch ein GUI. Dann kann die Funktion (1) verwendet werden.
// Werden die Optionen vorab nicht geparst muss Funktion (2) verwendet werden.
int parseOptions(int argc, wchar_t** argv, Options& options);
int start_xray(GUIcomm* guiComm=NULL); // (1)
int start_xray(int argc, wchar_t** argv, GUIcomm* guiComm=NULL); // (2)




#endif//X_RAY_H_