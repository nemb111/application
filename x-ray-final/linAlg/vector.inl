/*
 * vector.inl
 *
 *  Created on: 12.11.2012
 *  Author: Benedikt Mendorf
 */
#include <iostream>


inline const FLOATING&
HVector::operator[](size_t idx) const
{
  switch(idx)
  {
   case 0:
     return x;
   case 1:
     return y;
   case 2:
     return z;
   case 3:
     return w;
  }

  return x;
}

inline const FLOATING&
HVector::operator[](const std::string& str) const
{
  if (str.compare("x") )
    return x;
  else if (str.compare("y") )
    return y;
  else if (str.compare("z") )
    return z;
  else if (str.compare("w") )
    return w;
  else 
  {
    std::cerr <<"ERROR in: " << __FUNCTION__ << " ungültiger index."
              << std::endl;
    std::cin.get();
    exit(1); return x;
  }
}

inline FLOATING&
HVector::operator[](size_t idx)
{
  switch(idx)
  {
   case 0:
     return x;
   case 1:
     return y;
   case 2:
     return z;
   case 3:
     return w;
  }

  return x;
}

inline FLOATING&
HVector::operator[](const std::string& str)
{
  if (str.compare("x") )
    return x;
  else if (str.compare("y") )
    return y;
  else if (str.compare("z") )
    return z;
  else if (str.compare("w") )
    return w;
  else {std::cerr << __FUNCTION__ << "ungültiger index." << std::endl;
        std::cin.get(); exit(1); return x;}
}

inline bool 
HVector::operator==(const HVector& rhs) const
{
  return( gutils::equals(x, rhs.x) && gutils::equals(y, rhs.y) &&
          gutils::equals(z, rhs.z) && gutils::equals(w, rhs.w) );
}

inline bool
HVector::operator!=(const HVector& rhs) const
{
  return !(operator==(rhs) );
}

inline FLOATING
HVector::length() const
{
  FLOATING scale = (gutils::equals(w, (FLOATING)0.) ) ? 1 : (FLOATING)1./w;
  return sqrt( pow(x*scale, 2) + pow(y*scale, 2) + pow(z*scale, 2) );
}

inline FLOATING
HVector::length2() const
{
  FLOATING scale = (gutils::equals(w, (FLOATING)0.) ) ? 1 : (FLOATING)1./w;
  return pow(x*scale, 2) + pow(y*scale, 2) + pow(z*scale, 2);
}


/***********************************Point*************************************/

inline Point
Point::operator +(const Vector& rhs) const
{
  Point tmp(*this);
  tmp += rhs;

  return tmp;
}

Point&
Point::operator +=(const Vector& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;

  return *this;
}

Vector
Point::operator -(const Point& rhs) const
{
  Vector tmp(this->x/this->w - rhs.x/rhs.w, this->y/this->w - rhs.y/rhs.w,
             this->z/this->w - rhs.z/rhs.w);

  return tmp;
}

Point
Point::operator*(const FLOATING& rhs) const
{
  Point tmp(*this);
  tmp *= rhs;

  return tmp;
}
 
Point&
Point::operator*=(const FLOATING& rhs)
{
  this->x *= rhs;
  this->y *= rhs;
  this->z *= rhs;

  return *this;
}

Point
Point::baryAdd(const Point& rhs) const
{
  Point tmp(*this);

  tmp.x += rhs.x;
  tmp.y += rhs.y;
  tmp.z += rhs.z;
  tmp.w += rhs.w;

  return tmp;
}

inline Point
Point::normW() const
{
  FLOATING w_inv = (FLOATING)(1./w);
  return Point(x*w_inv, y*w_inv, z*w_inv);
}

inline const Point&
Point::normWIP()
{
  FLOATING w_inv = (FLOATING)1./w;
  x *= w_inv;
  y *= w_inv;
  z *= w_inv;
  w *= w_inv;

  return *this;
}

Point
operator +(const Vector& lhs, const Point& rhs)
{
  Point tmp(rhs);
  tmp += lhs;

  return tmp;
}

Point
operator*(const FLOATING& lhs, const Point& rhs)
{
  Point tmp(rhs);
  tmp *= lhs;

  return tmp;
}

/********************************Vector***************************************/

Vector
Vector::operator +(const Vector& rhs) const
{
  Vector tmp(*this);
  tmp += rhs;

  return tmp;
}

Vector&
Vector::operator +=(const Vector& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;

  return *this;
}

Vector
Vector::operator-(const Vector& rhs) const
{
  Vector tmp(*this);
  tmp -= rhs;

  return tmp;
}

Vector&
Vector::operator-=(const Vector& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;

  return *this;
}

Vector
Vector::operator*(const FLOATING& rhs) const
{
  Vector tmp(*this);
  tmp *= rhs;

  return tmp;
}

FLOATING
Vector::operator*(const Vector& rhs) const
{
  return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vector&
Vector::operator*=(const FLOATING& rhs)
{
  this->x *= rhs;
  this->y *= rhs;
  this->z *= rhs;

  return *this;

}

inline Vector
Vector::norm() const
{
  //gutils_assert(!gutils::equals(this->length(), 0.) );

  FLOATING length = this->length();
  FLOATING length_inv = (gutils::equals(length, (FLOATING)0.) ) ? 1 : (FLOATING)1./length;
  return Vector(x*length_inv, y*length_inv, z*length_inv);
}


inline const Vector&
Vector::normIP()
{
  //gutils_assert(!gutils::equals(this->length(), 0.) );

  FLOATING length = this->length();
  FLOATING length_inv = (gutils::equals(length, (FLOATING)0.) ) ? 1 : (FLOATING)1./length;
  x *= length_inv;
  y *= length_inv;
  z *= length_inv;

  return *this;
}

inline const Normal& 
Normal::operator=(const Vector& vec)
{
  gutils_assert(gutils::equals(vec.w, (FLOATING)0.) );

  x = vec.x;
  y = vec.y;
  z = vec.z;
  this->normIP();

  return *this;
}

inline FLOATING&
Normal::operator[](int idx)
{
  gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}

inline FLOATING& 
Normal::operator[](const std::string& str)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}

inline Vector& 
Normal::operator+=(const Vector& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}

inline Vector&
Normal::operator -=(const Vector& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}

inline Vector&
Normal::operator*=(const FLOATING& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}

inline void
Normal::invIP()
{
  x *= -1;
  y *= -1;
  z *= -1;
}

inline Normal
Normal::inv() const
{
  return Normal(-x, -y, -z);
}

Vector
operator*(const FLOATING& lhs, const Vector& rhs)
{
  Vector tmp(rhs);
  tmp *= lhs;

  return tmp;
}

inline Vector 
cross(const Vector& a, const Vector& b)
{
  return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

inline FLOATING
angle(const Vector& a, const Vector& b)
{
  gutils_assert(a.length2() > 0. && b.length2() > 0.);

  return acos(a*b / (a.length() * b.length() ) );
}

inline FLOATING
angle(const Normal& a, const Normal& b)
{
  gutils_assert(a.length2() > 0. && b.length2() > 0.);

  return acos(a*b);
}

inline void
orthNormBasis(Vector& e1, Vector& e2, Vector& e3)
{
  gutils_assert(cross(e1, e2) != Vector(0,0,0) );

  e3 = cross(e1, e2);
  e1.normIP();
  e3.normIP();
  
  e2 = cross(e1, -1 * e3);
  e2.normIP();
}