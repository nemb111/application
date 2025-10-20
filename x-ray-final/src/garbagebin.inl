

//_____________________________________________________________________________
template<class T>
 void
GarbageBin::mrkDel(T pointer)
{
  int mutex_result = WaitForSingleObject(_mrkDelMutex,      //handle to mutex
                                         INFINITE);   //no time-out interval

#ifndef NDEBUG
  switch (mutex_result) 
  {
     // The 	acquiring of the mutex failed. Possible problem: mutex not
    // initialized
    case WAIT_FAILED:
    {
      EOUT("Der Mutex konnte nicht erworben werden");
      std::cin.get();
      exit(1);
    }
    // The thread got ownership of the mutex
    case WAIT_OBJECT_0: 
      break; 
    // The thread got ownership of an abandoned mutex
    // The database is in an indeterminate state
    case WAIT_ABANDONED:
      WOUT("Es wurde ein \"abandoned mutex\" erworben.");
      break; 
  }
#endif //NDEBUG


  if (typeid(Accelerator*) == typeid(pointer) )
    _accPointer.push_back(dynamic_cast<Accelerator*>(pointer) );
  else if (typeid(Camera*) == typeid(pointer) )
    _camPointer.push_back(dynamic_cast<Camera*>(pointer) );
  else if (typeid(Light*) == typeid(pointer) )
    _lgtPointer.push_back(dynamic_cast<Light*>(pointer) );
  else if (typeid(Material*) == typeid(pointer) )
    _matPointer.push_back(dynamic_cast<Material*>(pointer) );
  else if (typeid(MeshParser*) == typeid(pointer) )
    _mepPointer.push_back(dynamic_cast<MeshParser*>(pointer) );
  else if (typeid(NormalMap*) == typeid(pointer) )
    _nmpPointer.push_back(dynamic_cast<NormalMap*>(pointer) );
  else if (typeid(Shape*) == typeid(pointer) )
    _shpPointer.push_back(dynamic_cast<Shape*>(pointer) );
  else if (typeid(Texture*) == typeid(pointer) )
    _texPointer.push_back(dynamic_cast<Texture*>(pointer) );
  else if (typeid(UnitSampler*) == typeid(pointer) )
    _uspPointer.push_back(dynamic_cast<UnitSampler*>(pointer) );
  else
  {
    WOUT("Ein Zeiger mit diesem Typ kann nicht zum Löschen vorgemerkt werden.");
  }


#ifndef NDEBUG
  if (ReleaseMutex(_mrkDelMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_mrkDelMutex);
#endif //NDEBUG

}


//_____________________________________________________________________________
template<class T>
 void
GarbageBin::mrkDelA(T pointer)
{
  int mutex_result = WaitForSingleObject(_mrkDelAMutex,      //handle to mutex
                                         INFINITE);   //no time-out interval

#ifndef NDEBUG
  switch (mutex_result) 
  {
     // The 	acquiring of the mutex failed. Possible problem: mutex not
    // initialized
    case WAIT_FAILED:
    {
      EOUT("Der Mutex konnte nicht erworben werden");
      std::cin.get();
      exit(1);
    }
    // The thread got ownership of the mutex
    case WAIT_OBJECT_0: 
      break; 
    // The thread got ownership of an abandoned mutex
    // The database is in an indeterminate state
    case WAIT_ABANDONED:
      WOUT("Es wurde ein \"abandoned mutex\" erworben.");
      break; 
  }
#endif //NDEBUG


  if (typeid(Accelerator*) == typeid(pointer) )
    _accPointerA.push_back(dynamic_cast<Accelerator*>(pointer) );
  else if (typeid(Camera*) == typeid(pointer) )
    _camPointerA.push_back(dynamic_cast<Camera*>(pointer) );
  else if (typeid(Light*) == typeid(pointer) )
    _lgtPointerA.push_back(dynamic_cast<Light*>(pointer) );
  else if (typeid(Material*) == typeid(pointer) )
    _matPointerA.push_back(dynamic_cast<Material*>(pointer) );
  else if (typeid(MeshParser*) == typeid(pointer) )
    _mepPointerA.push_back(dynamic_cast<MeshParser*>(pointer) );
  else if (typeid(NormalMap*) == typeid(pointer) )
    _nmpPointerA.push_back(dynamic_cast<NormalMap*>(pointer) );
  else if (typeid(Shape*) == typeid(pointer) )
    _shpPointerA.push_back(dynamic_cast<Shape*>(pointer) );
  else if (typeid(Texture*) == typeid(pointer) )
    _texPointerA.push_back(dynamic_cast<Texture*>(pointer) );
  else if (typeid(UnitSampler*) == typeid(pointer) )
    _uspPointerA.push_back(dynamic_cast<UnitSampler*>(pointer) );
  else
  {
    WOUT("Ein Zeiger mit diesem Typ kann nicht zum Löschen vorgemerkt werden.");
  }


#ifndef NDEBUG
  if (ReleaseMutex(_mrkDelAMutex) == 0)
  {
    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
    std::cin.get();
    exit(1);
  }
#else
  ReleaseMutex(_mrkDelAMutex);
#endif //NDEBUG

}