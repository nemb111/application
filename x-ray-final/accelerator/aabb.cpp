
#include "stdafx.h"

//#include <algorithm>
//#include <limits>
//
//#include "constants.h"
//#include "aabb.h"

//_____________________________________________________________________________
AABB::AABB()
{
  for (int i=0; i<3; i++)
  {
    minSlab[i] = std::numeric_limits<float>::max();
    maxSlab[i] = -std::numeric_limits<float>::max();
  }
  minSlab[3] = 0;
  maxSlab[3] = 0;
}



//_____________________________________________________________________________
AABB::AABB(float xMin, float xMax, float yMin, float yMax, float zMin, 
           float zMax)
{
  minSlab[0] = xMin;
  minSlab[1] = yMin;
  minSlab[2] = zMin;
  minSlab[3] = 0;

  maxSlab[0] = xMax;
  maxSlab[1] = yMax;
  maxSlab[2] = zMax;
  maxSlab[3] = 0;
}


//_____________________________________________________________________________
AABB::~AABB()
{
}


//_____________________________________________________________________________
AABB
AABB::empty()
{
  return AABB(
          std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(),
          std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(),
          std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()
         );
}


//_____________________________________________________________________________
bool
AABB::hit(const Ray& ray) const
{
  float tmin, tmax;
  return hit(ray, tmin, tmax);
}


// Alte Version bleibt hier stehen fürs Verständnis. Unten die optimierte
// Variante
//_____________________________________________________________________________
//bool
//AABB::hit(const Ray& ray, float& tmin, float& tmax) const
//{
//  float t0 = static_cast<float>(KEPSILON);
//  float t1 = std::numeric_limits<float>::max();
//
//  for (int i=0; i<3; i++)
//  {
//    float invDir = static_cast<float>(1. / ray.d()[i]);
//    float tNear = static_cast<float>((minSlab[i] - ray.o()[i]) * invDir);
//    float tFar  = static_cast<float>((maxSlab[i] - ray.o()[i]) * invDir);
//
//    if (tNear > tFar) std::swap(tNear, tFar);
//    t0 = tNear > t0 ? tNear : t0;
//    t1 = tFar  < t1 ? tFar  : t1;
//    if (t0 > t1) return false;
//  }
//  tmin = t0;
//  tmax = t1;
//
//  return true;
//}


//_____________________________________________________________________________
bool
AABB::hit(const Ray& ray, float& tmin, float& tmax) const
{
  // http://www.flipcode.com/archives/SSE_RayBox_Intersection_Test.shtml
  // turn those verbose intrinsics into something readable.
#define loadps(mem)		_mm_load_ps((const float * const)(mem))
#define storess(ss,mem)		_mm_store_ss((float * const)(mem),(ss))
#define minss			_mm_min_ss
#define maxss			_mm_max_ss
#define minps			_mm_min_ps
#define maxps			_mm_max_ps
#define mulps			_mm_mul_ps
#define subps			_mm_sub_ps
#define rotatelps(ps)		_mm_shuffle_ps((ps),(ps), 0x39)	// a,b,c,d -> b,c,d,a
#define muxhps(low,high)	_mm_movehl_ps((low),(high))	// low{a,b,c,d}|high{e,f,g,h} = {c,d,g,h}
  static const float flt_plus_inf = -logf(0);	// let's keep C and C++ compilers happy.
  static const float __declspec(align(16))
    ps_cst_plus_inf[4] = { flt_plus_inf, flt_plus_inf, flt_plus_inf, flt_plus_inf },
    ps_cst_minus_inf[4] = { -flt_plus_inf, -flt_plus_inf, -flt_plus_inf, -flt_plus_inf };


  float __declspec(align(16)) d[4] = 
    { static_cast<float>(ray.d()[0]), static_cast<float>(ray.d()[1]), 
      static_cast<float>(ray.d()[2]), 1 };
  float __declspec(align(16)) o[4] = 
    { static_cast<float>(ray.o()[0]), static_cast<float>(ray.o()[1]),
      static_cast<float>(ray.o()[2]), 0 };

  // you may already have those values hanging around somewhere
  const __m128
    plus_inf = loadps(ps_cst_plus_inf),
    minus_inf = loadps(ps_cst_minus_inf);

  // use whatever's apropriate to load.
  const __m128
    box_min = loadps(minSlab ),
    box_max = loadps(maxSlab ),
    pos = loadps(o),
    inv_dir = _mm_div_ps(_mm_set_ps(0.f, 1.f, 1.f, 1.f), loadps(d) );

  // use a div if inverted directions aren't available
  const __m128 l1 = mulps(subps(box_min, pos), inv_dir);
  const __m128 l2 = mulps(subps(box_max, pos), inv_dir);

  // the order we use for those min/max is vital to filter out
  // NaNs that happens when an inv_dir is +/- inf and
  // (box_min - pos) is 0. inf * 0 = NaN
  const __m128 filtered_l1a = minps(l1, plus_inf);
  const __m128 filtered_l2a = minps(l2, plus_inf);

  const __m128 filtered_l1b = maxps(l1, minus_inf);
  const __m128 filtered_l2b = maxps(l2, minus_inf);

  // now that we're back on our feet, test those slabs.
  __m128 lmax = maxps(filtered_l1a, filtered_l2a);
  __m128 lmin = minps(filtered_l1b, filtered_l2b);

  // unfold back. try to hide the latency of the shufps & co.
  const __m128 lmax0 = rotatelps(lmax);
  const __m128 lmin0 = rotatelps(lmin);
  lmax = minss(lmax, lmax0);
  lmin = maxss(lmin, lmin0);

  const __m128 lmax1 = muxhps(lmax, lmax);
  const __m128 lmin1 = muxhps(lmin, lmin);
  lmax = minss(lmax, lmax1);
  lmin = maxss(lmin, lmin1);

  const bool ret = (_mm_comige_ss(lmax, _mm_setzero_ps()) & _mm_comige_ss(lmax, lmin)) != 0;

  storess(lmin, &tmin);
  storess(lmax, &tmax);


  return  ret;

}


//_____________________________________________________________________________
void
AABB::extend(const Point& point)
{
  minSlab[0] = std::min(minSlab[0], static_cast<float>(point.x) );
  minSlab[1] = std::min(minSlab[1], static_cast<float>(point.y) );
  minSlab[2] = std::min(minSlab[2], static_cast<float>(point.z) );
  maxSlab[0] = std::max(maxSlab[0], static_cast<float>(point.x) );
  maxSlab[1] = std::max(maxSlab[1], static_cast<float>(point.y) );
  maxSlab[2] = std::max(maxSlab[2], static_cast<float>(point.z) );
}


//_____________________________________________________________________________
void
AABB::extend(const AABB& aabb)
{
  minSlab[0] = std::min(minSlab[0], aabb.minSlab[0] );
  minSlab[1] = std::min(minSlab[1], aabb.minSlab[1] );
  minSlab[2] = std::min(minSlab[2], aabb.minSlab[2] );
  maxSlab[0] = std::max(maxSlab[0], aabb.maxSlab[0] );
  maxSlab[1] = std::max(maxSlab[1], aabb.maxSlab[1] );
  maxSlab[2] = std::max(maxSlab[2], aabb.maxSlab[2] );
}


//_____________________________________________________________________________
float
AABB::getArea() const
{
  float length = fabs(maxSlab[0] - minSlab[0]);
  float height = fabs(maxSlab[1] - minSlab[1]);
  float width  = fabs(maxSlab[2] - minSlab[2]);

  return (length+width+height)/3.0f;
}


//_____________________________________________________________________________
Point
AABB::getCentroid() const
{
  return Point(minSlab[0] + .5f*(maxSlab[0]-minSlab[0]),
               minSlab[1] + .5f*(maxSlab[1]-minSlab[1]),
               minSlab[2] + .5f*(maxSlab[2]-minSlab[2]) );
}


//_____________________________________________________________________________
Vector
AABB::diagonal() const
{
  return Vector(maxSlab[0]-minSlab[0], maxSlab[1]-minSlab[1],
                maxSlab[2]-minSlab[2]);
}