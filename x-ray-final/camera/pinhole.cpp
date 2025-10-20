#include "stdafx.h"

//#include "pinhole.h"

//_____________________________________________________________________________
//Pinhole::Pinhole(const Film* film, ViewPlaneSampler* viewPlaneSampler)
// : Camera(film, viewPlaneSampler), _planePos(Point(0,0,0) ),
//   _eyePos(Point(0,0,50) ), _lookDir(Vector(0,0,-1) ), _upVec(Vector(0,1,0) ),
//   _distToFilm(50), _e1(Vector(1,0,0) ), _e2(Vector(0,1,0) ), _e3(Vector(0,0,1) )
//{
//}


//_____________________________________________________________________________
Pinhole::Pinhole()
  : Camera(), _planePos(Point(0,0,0) ), _eyePos(Point(0,0,50) ),
    _lookDir(Vector(0,0,-1) ), _upVec(Vector(0,1,0) ), _distToFilm(50),
    _e1(Vector(1,0,0) ), _e2(Vector(0,1,0) ), _e3(Vector(0,0,1) )
{
}


//_____________________________________________________________________________
Pinhole::Pinhole(const Point& planePos, const Normal& lookDir, 
                 const Normal& upVec, double distToFilm)
 : Camera(), _planePos(planePos),
   _eyePos(planePos + lookDir.norm() * -distToFilm), _lookDir(lookDir),
   _upVec(upVec), _distToFilm(distToFilm)
{
  assert(lookDir != Normal(0,0,0) && upVec != Normal(0,0,0) );

  createONB(_upVec, _lookDir);
}


//_____________________________________________________________________________
Pinhole::Pinhole(const Point& planePos, const Normal& lookDir,  
                 const Normal& upVec, double viewPlaneWidth,
                 double horizontalFOV)
  : Camera(), _planePos(planePos), _lookDir(lookDir), _upVec(upVec)
{
  assert(lookDir != Normal(0,0,0) && upVec != Normal(0,0,0) );

  _distToFilm = (viewPlaneWidth / (2. * tan(horizontalFOV / 2.) ) );
  _eyePos = (_planePos + _lookDir * -_distToFilm);
  
  createONB(_upVec, _lookDir);
}


//_____________________________________________________________________________
Pinhole::~Pinhole(void)
{
}


//_____________________________________________________________________________
//bool
//Pinhole::getNextNSamples(uint nSamples, SampleList& sampleList)
//{
//  double xOff, yOff;
//  PxlSampleList pxlSampleList;
//  
//  bool result = Camera::getNextNSamples(nSamples, sampleList);
//
//  for (size_t i=0; i<sampleList.size(); i++)
//  {
//    pxlSampleList.clear();
//    _dIllViewPlaneS.getPxlSamples(pxlSampleList);
//    for (size_t j=0; j<pxlSampleList.size(); j++)
//    {
//      getPxlOffset(sampleList[i].pxlIdxX, sampleList[i].pxlIdxY, xOff, yOff);
//      // Richtungsvektor d ist in globalen Koordinaten definiert
//      Vector d(_film->getPxlSize()*(xOff+pxlSampleList[j].xcoord), 
//               _film->getPxlSize()*(yOff+pxlSampleList[j].ycoord), -_distToFilm);
//      // Erzeuge aus Vektor d und dem ONB einen Vektor dONB in Kamerakoordinaten
//      Normal dONB = _e1 * d.x + _e2 * d.y + _e3 * d.z;
//      sampleList[i].rays.push_back(Ray(_eyePos, dONB) );
//      sampleList[i].weigths.push_back(pxlSampleList[j].weight);
//    }
//  }
//
//  return result;
//}


//_____________________________________________________________________________
bool
Pinhole::getNextNSamples(uint nSamples, SampleList& dirIllSampleList,
                         SampleList& globIllSampleList)
{
  double xOff, yOff;
  PxlSampleList pxlSampleList;
  
  bool result = Camera::getNextNSamples(nSamples, dirIllSampleList);
  

  globIllSampleList = dirIllSampleList;


  for (size_t i=0; i<dirIllSampleList.size(); i++)
  {
    pxlSampleList.clear();
    _dIllViewPlaneS.getPxlSamples(pxlSampleList);
    for (size_t j=0; j<pxlSampleList.size(); j++)
    {
      getPxlOffset(dirIllSampleList[i].pxlIdxX, dirIllSampleList[i].pxlIdxY,
                   xOff, yOff);
      // Richtungsvektor d ist in globalen Koordinaten definiert
      Vector d(_film->getPxlSize()*(xOff+pxlSampleList[j].xcoord), 
               _film->getPxlSize()*(yOff+pxlSampleList[j].ycoord), -_distToFilm);
      // Erzeuge aus Vektor d und dem ONB einen Vektor dONB in Kamerakoordinaten
      Normal dONB = _e1 * d.x + _e2 * d.y + _e3 * d.z;
      dirIllSampleList[i].rays.push_back(Ray(_eyePos, dONB) );
      dirIllSampleList[i].weigths.push_back(pxlSampleList[j].weight);
    }
  }


  for (size_t i=0; i<globIllSampleList.size(); i++)
  {
    pxlSampleList.clear();
    _gIllViewPlaneS.getPxlSamples(pxlSampleList);
    for (size_t j=0; j<pxlSampleList.size(); j++)
    {
      getPxlOffset(globIllSampleList[i].pxlIdxX, globIllSampleList[i].pxlIdxY,
                   xOff, yOff);
      // Richtungsvektor d ist in globalen Koordinaten definiert
      Vector d(_film->getPxlSize()*(xOff+pxlSampleList[j].xcoord), 
               _film->getPxlSize()*(yOff+pxlSampleList[j].ycoord), -_distToFilm);
      // Erzeuge aus Vektor d und dem ONB einen Vektor dONB in Kamerakoordinaten
      Normal dONB = _e1 * d.x + _e2 * d.y + _e3 * d.z;
      globIllSampleList[i].rays.push_back(Ray(_eyePos, dONB) );
      globIllSampleList[i].weigths.push_back(pxlSampleList[j].weight);
    }
  }

  return result;
}


//_____________________________________________________________________________
void
Pinhole::lookAt(const Point& lookAt, const Normal& upVec/*=Vector(0,1,0)*/ )
{
  assert(upVec != Normal(0,0,0) );

  _lookDir = (lookAt - _planePos == Vector(0,0,0) ) ? Vector(0,0,-1) : 
              lookAt - _planePos;
  _eyePos = _planePos + _lookDir * -_distToFilm;
  _upVec   = upVec;

  //Behandelt den speziellen Fall, in dem der upVec unverändert bleibt und
  //man genau senkrecht nach oben oder unten schaut
  if (_upVec == Vector(0,1,0) && (_upVec == _lookDir ||
      _upVec == _lookDir.inv() ) )
    _upVec = Vector(0,0,-1);

  createONB(_upVec, _lookDir);
}


//_____________________________________________________________________________
void
Pinhole::setPos(const Point& planePos, const Normal& upVec/*=Vector(0,1,0)*/ )
{
  assert(upVec != Normal(0,0,0) );

  _planePos = planePos;
  _eyePos = planePos + _lookDir * -_distToFilm;
  _upVec  = upVec;
  createONB(_upVec, _lookDir);
}


//_____________________________________________________________________________
void
Pinhole::setFilmDist(double distToFilm)
{
  _distToFilm = distToFilm;
}


//_____________________________________________________________________________
void
Pinhole::createONB(const Normal& upVec, const Normal& lookDir)
{
  _e2 = upVec;
  _e3 = lookDir.inv();
  orthNormBasis(_e3, _e2, _e1);
  _e1.invIP();
}




//_____________________________________________________________________________

//_____________________________________________________________________________