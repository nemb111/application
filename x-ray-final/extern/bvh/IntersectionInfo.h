#ifndef IntersectionInfo_h_
#define IntersectionInfo_h_

#include "Vector3.h"
#include "../../src/ray.h"


namespace BVHNS
{

struct Object;

struct IntersectionInfo {
 float t; // Intersection distance along the ray
 const Object* object; // Object that was hit
 Vector3 normal; // the normal at the hit position
 Vector3 hit; // Location of the intersection

 // Nachträglich eingefügt. Zur Dürchführung des Strahl-Primitven Schnitttests
 // mit dem originalen Strahl. D.h. ohne Konvertierung zu float.
 ::Ray ray;
 bool shadowHit;
};

}
#endif
