#pragma once

#include <Windows.h>

#include <iostream>
#include <random>

#include "options.h"

#ifndef MUTEXEDRNG_H_
#define MUTEXEDRNG_H_


class MutexedRNG
{
public:
 

  static void seed(unsigned long seed)
  {
    _rng.seed(seed);
  }


  template<typename T>
  static auto number(const T& distribution) -> decltype( distribution(_rng) )
  {
    int mutex_result = WaitForSingleObject(_mutex,      //handle to mutex
                                           INFINITE);   //no time-out interval
    switch (mutex_result) 
    {
      // The thread got ownership of the mutex
      case WAIT_OBJECT_0:
      {
        auto res = distribution(_rng);
        ReleaseMutex(_mutex);
        return res;
      }

      default:
        EOUT("Etwas ist beim erwerben des Mutex schief gegangen.");
        std::cin.get();
        exit(EXIT_FAILURE);
    }

  }

private:
  MutexedRNG(void);
  ~MutexedRNG(void);


  static std::mt19937 _rng;
  static HANDLE _mutex;

};


#endif //MUTEXEDRNG_H_