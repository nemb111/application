
#pragma once

#ifndef PHOTONPOOL_H_
#define PHOTONPOOL_H_

#include <memory>
#include <vector>

#include "photon.h"


class PhotonPool
{
public:
  //_____________________________________________________________________________
  PhotonPool()
    : _startCapacity(1000000)
  {
    _photonPool.reserve(_startCapacity * 2);
    for (unsigned int i=0; i<_startCapacity; i++)
    {
      _photonPool.push_back(std::move(UPTPhoton(new TracePhoton) ) );
    }
  }


  //_____________________________________________________________________________
  PhotonPool(unsigned int startCapacity)
    : _startCapacity(startCapacity)
  {
    _photonPool.reserve(_startCapacity * 2);
    for (unsigned int i=0; i<_startCapacity; i++)
    {
      _photonPool.push_back(std::move(UPTPhoton(new TracePhoton) ) );
    }
  }

  //_____________________________________________________________________________
  ~PhotonPool() {}


  //_____________________________________________________________________________
  void push(UPTPhoton uppPhoton)
  {
    _photonPool.push_back(std::move(uppPhoton) );
  }


  //_____________________________________________________________________________
  UPTPhoton pop()
  {
    UPTPhoton tmp;

    if (_photonPool.size() == 0)
      for (unsigned int i=0; i<_startCapacity; i++)
        _photonPool.push_back(UPTPhoton(new TracePhoton) );


    tmp = std::move(_photonPool.back() );
    _photonPool.pop_back();

    return tmp;
  }


  //_____________________________________________________________________________
  void clear()
  {
    _photonPool.clear();
  }


private:
  unsigned int _startCapacity;
  std::vector<UPTPhoton> _photonPool;

};


#endif //PHOTONPOOL_H_