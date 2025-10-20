#include "stdafx.h"

//#include "hemispheres.h"
//#include "parser.h"

//_____________________________________________________________________________
HemisphereS::HemisphereS(UnitSampler* unitSampler/*=nullptr*/, double e/*=0.*/)
  : _e(e), _unitSampler(unitSampler)
{
}


//_____________________________________________________________________________
HemisphereS::~HemisphereS(void)
{
}


//_____________________________________________________________________________
HemisphereS*
HemisphereS::clone() const
{
  UnitSampler* tmp = _unitSampler->clone();
  GBin.mrkDel<UnitSampler*>(tmp);
  HemisphereS* res = new HemisphereS(tmp, _e);


  return res;
}


//_____________________________________________________________________________
uint
HemisphereS::createSamples(std::vector<Point>& samplePoints)
{
  double x, y;
  uint nSamples = _unitSampler->getNumSamples();
  _unitSampler->reset();
  samplePoints.clear();
  samplePoints.reserve(nSamples );

  for (uint i=0; i<nSamples; i++)
  {
    _unitSampler->createSample(x, y);

    double cosPhi = std::cos(2.0 * PI * x);
    double sinPhi = std::sin(2.0 * PI * x);
    double cosTheta = std::pow((1.0 - y), 1.0 / (_e + 1.0) );
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    double pu = sinTheta * cosPhi;
    double pv = sinTheta * sinPhi;
    double pw = cosTheta;

    samplePoints.push_back(Point(pu, pv, pw) );
  }
  _unitSampler->reset();

  return static_cast<uint>(samplePoints.size() );
}


//_____________________________________________________________________________
uint
HemisphereS::getNumSamples() const
{
  return _unitSampler->getNumSamples();
}