#include "stdafx.h"

//#include "bvhwrapper.h"
//
//#include "aabb.h"
//#include "parser.h"

//_____________________________________________________________________________
bool 
ObjectWrapper::getIntersection(const BVHNS::Ray& ray, 
                               BVHNS::IntersectionInfo* intersection) const
{
  if (intersection->shadowHit == true && 
      _shape.getThrowsShadow() == false)
    return false;

  bool res;
  double tm = std::numeric_limits<double>::max();

  //if (intersection->shadowHit == false)
  //{
  //  World world;
  //  ShadeRec sr(world );

  //  res = _shape.hit(intersection->ray, tm, sr); 

  //  if (res && static_cast<float>(tm) < intersection->t)
  //  {
  //    intersection->t = static_cast<float>(tm);
  //    intersection->object = this;
  //  }

  //  return res;
  //}
  //else
  //{
  res = _shape.shadowHit(intersection->ray, tm);

  if (res && static_cast<float>(tm) < intersection->t)
  {
    intersection->t = static_cast<float>(tm);
    intersection->object = this;
  }

  return res;
  //}

}


//_____________________________________________________________________________
BVHNS::Vector3
ObjectWrapper::getNormal(const BVHNS::IntersectionInfo& I) const
{
  return I.normal;
}


//_____________________________________________________________________________
BVHNS::BBox
ObjectWrapper::getBBox() const
{
  const AABB& aabb = dynamic_cast<const AABB&>(_shape.getBV() );
  return BVHNS::BBox(BVHNS::Vector3(aabb.minSlab[0], aabb.minSlab[1],
                                    aabb.minSlab[2]),
                     BVHNS::Vector3(aabb.maxSlab[0], aabb.maxSlab[1],
                                    aabb.maxSlab[2]) );
}


//_____________________________________________________________________________
BVHNS::Vector3
ObjectWrapper::getCentroid() const
{
  const AABB& aabb = dynamic_cast<const AABB&>(_shape.getBV() );
  Point centroid = aabb.getCentroid();
  return BVHNS::Vector3(static_cast<float>(centroid.x),
                        static_cast<float>(centroid.y),
                        static_cast<float>(centroid.z) );
}







//_____________________________________________________________________________
BVHwrapper::BVHwrapper(void)
  : Accelerator(), _isInit(false), _bvh(NULL), _shadowBVH(NULL)
{
}


//_____________________________________________________________________________
BVHwrapper::BVHwrapper(const std::vector<Shape*>& shapes)
  : Accelerator(shapes), _isInit(false), _bvh(NULL), _shadowBVH(NULL)
{
}


//_____________________________________________________________________________
BVHwrapper::~BVHwrapper(void)
{
  for (size_t i=0; i<_objects.size(); i++)
  {
    delete _objects[i];
    _objects[i] = 0;
  }

  for (size_t i=0; i<_shadowObjects.size(); i++)
  {
    delete _shadowObjects[i];
    _objects[i] = 0;
  }

  delete _bvh;
  _bvh = 0;
  delete _shadowBVH;
  _shadowBVH = 0;

}


//_____________________________________________________________________________
Accelerator*
BVHwrapper::newInstance() const
{
  Accelerator* accel = new BVHwrapper;
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
bool
BVHwrapper::create()
{
  assert(_shapesRef->size() > 0);

  if (_isInit)
  {
    WOUT("Die create-Methode wurde bereits aufgerufen, nichts geschieht.");
    return false;
  }
  _isInit = true;

  _objects.reserve(_shapesRef->size() );
  _shadowObjects.reserve(_shapesRef->size() );
  for (size_t i=0; i<_shapesRef->size(); i++)
  {
    _objects.push_back(new ObjectWrapper(
                            const_cast<const Shape&>(*(*_shapesRef)[i]), i ) );
    if ((*_shapesRef)[i]->getThrowsShadow() == true)
      _shadowObjects.push_back(new ObjectWrapper(
                            const_cast<const Shape&>(*(*_shapesRef)[i]), i ) );
  }

  if (_objects.size() > 0)
    _bvh = new BVHNS::BVH(&_objects, 1);
  if (_shadowObjects.size() > 0)
    _shadowBVH = new BVHNS::BVH(&_shadowObjects, 1);


  return true;
}


//_____________________________________________________________________________
Accelerator*
BVHwrapper::clone() const
{
  Accelerator* accel = new BVHwrapper(*this);
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
bool
BVHwrapper::hit(const Ray& ray, size_t& shpIdx, double* tmin/*=NULL*/) const
{
  bool res;

  BVHNS::Ray ray3(BVHNS::Vector3(static_cast<float>(ray.o().x),
                                 static_cast<float>(ray.o().y),
                                 static_cast<float>(ray.o().z) ),
                  BVHNS::Vector3(static_cast<float>(ray.d().x),
                                 static_cast<float>(ray.d().y),
                                 static_cast<float>(ray.d().z ) ) );
  BVHNS::IntersectionInfo intInfo;
  intInfo.ray = ray;
  intInfo.shadowHit = false;

  res = _bvh->getIntersection(ray3, &intInfo, false);
  if (res)
    shpIdx = dynamic_cast<const ObjectWrapper*>(intInfo.object)->getIdx();

  if (tmin)
    *tmin = static_cast<float>(intInfo.t);
  

  return res;
}


//_____________________________________________________________________________
bool
BVHwrapper::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  bool res = false;
  size_t shpIdx;

  if (hit(ray, shpIdx) )
    res = (*_shapesRef)[shpIdx]->hit(ray, tmin, sr);


  return res;
}


//_____________________________________________________________________________
bool
BVHwrapper::shadowHit(const Ray& ray, double& tmin) const
{
  if (_shadowObjects.size() == 0)
    return false;

  bool res;

  BVHNS::Ray ray3(BVHNS::Vector3(static_cast<float>(ray.o().x),
                                 static_cast<float>(ray.o().y),
                                 static_cast<float>(ray.o().z) ),
                  BVHNS::Vector3(static_cast<float>(ray.d().x),
                                 static_cast<float>(ray.d().y),
                                 static_cast<float>(ray.d().z) ) );
  BVHNS::IntersectionInfo intInfo;
  intInfo.ray = Ray(Point(ray.o().x + 10*DELTA * ray.d().x,
                          ray.o().y + 10*DELTA * ray.d().y,
                          ray.o().z + 10*DELTA * ray.d().z),
                    ray.d() );
  intInfo.shadowHit = true;

  res = _shadowBVH->getIntersection(ray3, &intInfo, false);

  tmin = static_cast<float>(intInfo.t);
  
  if (tmin > KEPSILON && res)
    return true;
  else
    return false;
}
