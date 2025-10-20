#pragma once

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <cmath>

// Autor des Codes ist der Betreiber der Webseite:
// http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/


static inline void progressbar(unsigned int x, unsigned int n,
                               unsigned int w = 50)
{
    if (fmod(x/float(n), 1.f) < 0.01f || (x/(float)n >= 1.1f) ) 
    {
      return;
    }
 
    float ratio  =  x/(float)n;
    int   c      =  (int)(ratio * w);
 
    std::cout << std::setw(3) << (int)(ratio*100) << "% [";
    for (int x=0; x<c; x++) std::cout << "=";
    for (unsigned int x=c; x<w; x++) std::cout << " ";
    std::cout << "]\r" << std::flush;

}



#endif //PROGRESSBAR_H_