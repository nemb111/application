#include "BBox.h"
#include <algorithm>

namespace BVHNS
{

BBox::BBox(const Vector3& min, const Vector3& max)
: min(min), max(max) { extent = max - min; }

BBox::BBox(const Vector3& p)
: min(p), max(p) { extent = max - min; }
 
void BBox::expandToInclude(const Vector3& p) {
 min = ::BVHNS::min(min, p);
 max = ::BVHNS::max(max, p);
 extent = max - min;
}

void BBox::expandToInclude(const BBox& b) {
 min = ::BVHNS::min(min, b.min);
 max = ::BVHNS::max(max, b.max);
 extent = max - min;
}

uint32_t BBox::maxDimension() const {
 uint32_t result = 0;
 if(extent.y > extent.x) result = 1;
 if(extent.z > extent.y) result = 2;
 return result;
}

float BBox::surfaceArea() const {
 return 2.f*( extent.x*extent.z + extent.x*extent.y + extent.y*extent.z );
}

// Typical slab-based Ray-AABB test
bool BBox::intersect(const Ray& ray, float *tnear, float *tfar) const {
Vector3 tbot = ray.inv_d.cmul(min - ray.o);
Vector3 ttop = ray.inv_d.cmul(max - ray.o);

Vector3 tmin = ::BVHNS::min(ttop, tbot);
  Vector3 tmax = ::BVHNS::max(ttop, tbot);

  *tnear = std::max(std::max(tmin.x, tmin.y), tmin.z);
  *tfar = std::min(std::min(tmax.x,tmax.y), tmax.z);

  return !(*tnear > *tfar) && *tfar > 0;
}

}