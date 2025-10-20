#include "stdafx.h"

//#include "orthocamera.h"


//____________________________________________________________________________
void
OrthoCamera::getPxlMidPnt(uint pxlIdxX, uint pxlIdxY, uint xres, uint yres,
                          double pixelSize, double& xcoord, double& ycoord)
{
  xcoord = pixelSize * (pxlIdxX - (xres * 0.5) + 0.5);
  ycoord = pixelSize * (pxlIdxY - (yres * 0.5) + 0.5);
}


//_____________________________________________________________________________
OrthoCamera::OrthoCamera()
  : Camera()
{
}


//_____________________________________________________________________________
//OrthoCamera::OrthoCamera(const Film* film, ViewPlaneSampler* viewPlaneSampler)
//  : Camera(film, viewPlaneSampler)
//{
//}


//_____________________________________________________________________________
OrthoCamera::~OrthoCamera(void)
{
}


//_____________________________________________________________________________
bool 
 OrthoCamera::getNextNSamples(uint nSamples, SampleList& sampleList)
{
  bool result = Camera::getNextNSamples(nSamples, sampleList);
  for (size_t j=0; j<sampleList.size(); j++)
  {
    double xcoord, ycoord;
    getPxlMidPnt(sampleList[j].pxlIdxX, sampleList[j].pxlIdxY,
                 _film->getPxlWidth(), _film->getPxlHeight(), 
                 _film->getPxlSize(), xcoord, ycoord);
    sampleList[j].rays.push_back(Ray(Point(xcoord, ycoord, 0.), 
                                     Vector(0., 0., -1.) ) );
  }
  
  return result;
}

//_____________________________________________________________________________
int
OrthoCamera::getSampleLists(const int& ntasks, 
                            std::vector<SampleList>& sampleLists)
{
  assert(sampleLists.size() == 0);

  uint pxlPerTask = (_film->getPxlWidth() * _film->getPxlHeight() ) / ntasks;
  uint remainPxl  = (_film->getPxlWidth() * _film->getPxlHeight() ) % ntasks;
  SampleList sampleList;
  
  for (int i=0; i<ntasks; i++)
  {
    sampleList.clear();
    getNextNSamples(pxlPerTask, sampleList);
    //Jedes Sample wird einem Pixel zugeordnet. Die entsprechenden Rays werden
    //hinzugefügt.
    for (size_t j=0; j<sampleList.size(); j++)
    {
      double xcoord, ycoord;
      getPxlMidPnt(sampleList[j].pxlIdxX, sampleList[j].pxlIdxY,
                   _film->getPxlWidth(), _film->getPxlHeight(),
                   _film->getPxlSize(), xcoord, ycoord);
      sampleList[j].rays.push_back(Ray(Point(xcoord, ycoord, 0.),
                                   Vector(0., 0., -1.) ) );
    }
    
    sampleLists.push_back(sampleList);
  }
  if (remainPxl)
  {
    sampleList.clear();
    getNextNSamples(remainPxl, sampleList);
    for (size_t j=0; j<sampleList.size(); j++)
    {
      double xcoord, ycoord;
      getPxlMidPnt(sampleList[j].pxlIdxX, sampleList[j].pxlIdxY,
                   _film->getPxlWidth(), _film->getPxlHeight(), 
                   _film->getPxlSize(), xcoord, ycoord);
      sampleList[j].rays.push_back(Ray(Point(xcoord, ycoord, 0.),
                                       Vector(0., 0., -1.) ) );
    }
    sampleLists.push_back(sampleList);
  }

  return 0;
}

//_____________________________________________________________________________
//bool
//OrthoCamera::getRayOrigins(int pixelX, int pixelY, std::vector<Point>* points) const
//{
//  double xcoord = _film->getPxlSize() * (pixelX - (_film->getPxlWidth() * 0.5) + 0.5);
//  double ycoord = _film->getPxlSize() * (pixelY - (_film->getPxlHeight() * 0.5) + 0.5);
//  Point origin(xcoord, ycoord, -_distToFilm);
//  points->push_back(origin);
//
//  return true;
//}