#pragma once

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <Windows.h>

#include <map>
#include <string>
#include "x-ray.h"

namespace
{

class Stopwatch
{
public:
  // Startet die Zeitmessung und verwendet den übergebenen string zur
  // Identifikation der Startzeit
  static void start(const stdstring& string);
  // Stoppt die Zeitmessung und gibt die verstrichene Zeit in
  // Millisekunden zurück.
  static unsigned int end(const stdstring& string);

private:

  static std::map<const stdstring, unsigned int> _timeMap;
};


//_____________________________________________________________________________
std::map<const stdstring, unsigned int> Stopwatch::_timeMap;


//_____________________________________________________________________________
void
Stopwatch::start(const stdstring& string)
{
  std::pair<std::map<const stdstring, unsigned int>::iterator, bool> res;
  SYSTEMTIME st;
  GetSystemTime(&st);

  res = _timeMap.insert(std::pair<const stdstring, unsigned int>(string,
                        3600000 * st.wHour + 60000 * st.wMinute + 
                        1000 * st.wSecond + st.wMilliseconds) );

  if (res.second == false)
    WOUT(_T("Bezeichner: \"") << string << _T("\". Eine gleichnamige Stopuhr wurde")
         _T(" bereits gestartet, nichts geschieht.") );
}


//_____________________________________________________________________________
unsigned int
Stopwatch::end(const stdstring& string)
{
  std::map<const stdstring, unsigned int>::iterator element;
  SYSTEMTIME st;
  unsigned int end, result;

  GetSystemTime(&st);
  end = 3600000 * st.wHour + 60000 * st.wMinute + 1000 * st.wSecond + 
        st.wMilliseconds;

  element = _timeMap.find(string);
  if (element == _timeMap.end() )
  {
    WOUT(_T("Bezeichner: \"") << string << 
         _T("\". Eine Stoppuhr mit diesem Namen") << 
         _T(" wurde nicht gestartet.") );
    return 0;
  }

  result = end - element->second;
  _timeMap.erase(element);
  
  
  return result;
}

}
#endif //STOPWATCH_H_