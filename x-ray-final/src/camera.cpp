#include "stdafx.h"

//#include "camera.h"


//_____________________________________________________________________________
bool
Camera::getNextNSamples(uint nSamples, SampleList& sampleList)
{
  assert(sampleList.size() == 0); 

  uint counter = 0;
  uint pxlHeight = _film->getPxlHeight();
  uint pxlWidth  = _film->getPxlWidth();

  while (_nextPxlIdxY < pxlHeight)
  {
    sampleList.push_back(Sample(_nextPxlIdxX, _nextPxlIdxY) );
    if (_nextPxlIdxX < pxlWidth - 1)
      _nextPxlIdxX++;
    else
    {
      _nextPxlIdxX = 0;
      _nextPxlIdxY++;
    }

    if(++counter >= nSamples || _nextPxlIdxY == pxlHeight)
      return true;
  }


  return false;

}


//_____________________________________________________________________________
Camera::Camera()
  : _nextPxlIdxX(0), _nextPxlIdxY(0), _film(NULL)
{
  //assert(_film != NULL);
  //assert(_viewPlaneSampler != NULL);

}


//_____________________________________________________________________________
Camera::~Camera()
{
}


//_____________________________________________________________________________
void
Camera::getPxlOffset(uint pxlIdxX, uint pxlIdxY, double& xOff, double& yOff)
{
  xOff = (pxlIdxX - (_film->getPxlWidth() * 0.5) );
  yOff = (pxlIdxY - (_film->getPxlHeight() * 0.5) );
}


//_____________________________________________________________________________
void
Camera::setFilm(const Film* film)
{
  _film = film;
}


//_____________________________________________________________________________
//void
//Camera::setUnitSampler(UnitSampler* unitSampler)
//{
//  _viewPlaneSampler = ViewPlaneSampler(unitSampler);
//}


//_____________________________________________________________________________
void 
Camera::setUnitSampler(UnitSampler* dIllViewPlaneUS,
                       UnitSampler* gIllViewPlaneUS/*=nullptr*/)
{
  if (gIllViewPlaneUS == nullptr)
  {
    _dIllViewPlaneS = ViewPlaneSampler(dIllViewPlaneUS);
    _gIllViewPlaneS = ViewPlaneSampler(dIllViewPlaneUS);
  }
  else
  {
    _dIllViewPlaneS = ViewPlaneSampler(dIllViewPlaneUS);
    _gIllViewPlaneS = ViewPlaneSampler(gIllViewPlaneUS);
  }
}
