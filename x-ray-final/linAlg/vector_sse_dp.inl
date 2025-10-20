

#include <iostream>
#include <limits>


/******************************************************************************
******************************* PRECISION 2 ***********************************
******************************************************************************/
inline const FLOATING&
HVector::operator[](size_t idx) const
{
  gutils_assert(idx >= 0 && idx <= 3);

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


//_____________________________________________________________________________
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


//_____________________________________________________________________________
inline FLOATING&
HVector::operator[](size_t idx)
{
  gutils_assert(idx >= 0 && idx <= 3);

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


//_____________________________________________________________________________
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


//_____________________________________________________________________________
inline bool 
HVector::operator==(const HVector& rhs) const
{
  __m128d epsilon = _mm_set_pd(gutils::EPSILON, gutils::EPSILON);
  __m128d diffXY = _mm_sub_pd(xy, rhs.xy);
  __m128d diffZW = _mm_sub_pd(zw, rhs.zw);
  __m128d negDiffXY = _mm_sub_pd(rhs.xy, xy);
  __m128d negDiffZW = _mm_sub_pd(rhs.zw, zw);
  __m128d absDiffXY = _mm_max_pd(diffXY, negDiffXY);
  __m128d absDiffZW = _mm_max_pd(diffZW, negDiffZW);
  __m128d res1 = _mm_cmplt_pd(absDiffXY, epsilon);
  __m128d res2 = _mm_cmplt_pd(absDiffZW, epsilon);


  return (res1.m128d_f64[0] != 0 && res1.m128d_f64[1] != 0 &&
          res2.m128d_f64[0] != 0 && res2.m128d_f64[1] != 0);
}


//_____________________________________________________________________________
inline bool
HVector::operator!=(const HVector& rhs) const
{
  return !(operator==(rhs) );
}


//_____________________________________________________________________________
inline FLOATING
HVector::length() const
{
  return sqrt(length2() );
}


//_____________________________________________________________________________
inline FLOATING
HVector::length2() const
{
  __m128d res = _mm_add_pd(_mm_mul_pd(xy, xy),  _mm_mul_pd(zw, zw) );
  return res.m128d_f64[0] + res.m128d_f64[1];
}


/***********************************Point*************************************/
inline Point
Point::operator+(const Vector& rhs) const
{
  return Point(_mm_add_pd(xy, rhs.xy), _mm_add_pd(zw, rhs.zw) );
}


//_____________________________________________________________________________
Point&
Point::operator +=(const Vector& rhs)
{
  *this = *this + rhs;

  return *this;
}


//_____________________________________________________________________________
Vector
Point::operator-(const Point& rhs) const
{
  double wInv = 1./w;
  double wRHSInv = 1./rhs.w;

  __m128d wVec = _mm_set_pd(wInv, wInv);
  __m128d wRHSVec = _mm_set_pd(wRHSInv, wRHSInv);

  Vector tmp(_mm_sub_pd(_mm_mul_pd(xy, wVec), _mm_mul_pd(rhs.xy, wRHSVec) ), 
             _mm_sub_pd(_mm_mul_pd(zw, wVec), _mm_mul_pd(rhs.zw, wRHSVec) ) );


  return tmp;
}


//_____________________________________________________________________________
Point
Point::operator*(const FLOATING& rhs) const
{
  Point tmp(*this);
  tmp *= rhs;

  return tmp;
}
 

//_____________________________________________________________________________
Point&
Point::operator*=(const FLOATING& rhs)
{
  this->x *= rhs;
  this->y *= rhs;
  this->z *= rhs;

  return *this;
}


//_____________________________________________________________________________
Point
Point::baryAdd(const Point& rhs) const
{
  return Point(_mm_add_pd(xy, rhs.xy), _mm_add_pd(zw, rhs.zw) );
}


//_____________________________________________________________________________
inline FLOATING
Point::length() const
{
  return sqrt( length2() );
}


//_____________________________________________________________________________
inline FLOATING
Point::length2() const
{
  FLOATING scale = (gutils::equals<FLOATING>(w, (FLOATING)0.) ) ? 1 : 1.f/w;
  return pow(x*scale, 2) + pow(y*scale, 2) + pow(z*scale, 2);
}


//_____________________________________________________________________________
inline Point
Point::normW() const
{
  __m128d wInvVec = _mm_set_pd(1./w, 1./w);
  return Point(_mm_mul_pd(xy, wInvVec), _mm_mul_pd(zw, wInvVec) );
}


//_____________________________________________________________________________
inline const Point&
Point::normWIP()
{
  __m128d wInvVec = _mm_set_pd(1./w, 1./w);
  xy = _mm_mul_pd(xy, wInvVec);
  zw = _mm_mul_pd(zw, wInvVec);

  return *this;
}


//_____________________________________________________________________________
Point
operator +(const Vector& lhs, const Point& rhs)
{
  Point tmp(rhs);
  tmp += lhs;

  return tmp;
}


//_____________________________________________________________________________
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
  return Vector(_mm_add_pd(xy, rhs.xy), _mm_add_pd(zw, rhs.zw) );
}


//_____________________________________________________________________________
Vector&
Vector::operator +=(const Vector& rhs)
{
  *this = *this + rhs;

  return *this;
}


//_____________________________________________________________________________
Vector
Vector::operator-(const Vector& rhs) const
{
  return Vector(_mm_sub_pd(xy, rhs.xy), _mm_sub_pd(zw, rhs.zw) );
}


//_____________________________________________________________________________
Vector&
Vector::operator-=(const Vector& rhs)
{
  *this = *this - rhs;

  return *this;
}


//_____________________________________________________________________________
Vector
Vector::operator*(const FLOATING& rhs) const
{
  __m128d rhsVec = _mm_set_pd(rhs, rhs);
  return Vector(_mm_mul_pd(xy, rhsVec), _mm_mul_pd(zw, rhsVec) );
}


//_____________________________________________________________________________
FLOATING
Vector::operator*(const Vector& rhs) const
{
  __m128d res = _mm_add_pd(_mm_mul_pd(xy, rhs.xy),
                           _mm_mul_pd(zw, rhs.zw) );
  return res.m128d_f64[0] + res.m128d_f64[1];
}


//_____________________________________________________________________________
Vector&
Vector::operator*=(const FLOATING& rhs)
{
  *this = *this * rhs;

  return *this;
}


//_____________________________________________________________________________
inline Vector
Vector::norm() const
{
  FLOATING length = this->length();
  FLOATING length_inv = (gutils::equals<FLOATING>(length, (FLOATING)0.) ) ?
    1 : 1.f/length;
  __m128d res = _mm_set_pd(length_inv, length_inv);
  return Vector(_mm_mul_pd(xy, res), _mm_mul_pd(zw, res) );
}


//_____________________________________________________________________________
inline const Vector&
Vector::normIP()
{
  FLOATING length = this->length();
  FLOATING length_inv = (gutils::equals<FLOATING>(length, (FLOATING)0.) ) ?
    1 : (FLOATING)1./length;
  __m128d res = _mm_set_pd(length_inv, length_inv);
  xy = _mm_mul_pd(xy, res);
  zw = _mm_mul_pd(zw, res);

  return *this;
}


/********************************Normal***************************************/
inline const Normal& 
Normal::operator=(const Vector& vec)
{
  gutils_assert(gutils::equals<FLOATING>(vec.w, (FLOATING)0.) );

  xy = vec.xy;
  zw = vec.zw;
  this->normIP();

  return *this;
}


//_____________________________________________________________________________
inline FLOATING&
Normal::operator[](int idx)
{
  gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}


//_____________________________________________________________________________
inline FLOATING& 
Normal::operator[](const std::string& str)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}


//_____________________________________________________________________________
inline Vector& 
Normal::operator+=(const Vector& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}


//_____________________________________________________________________________
inline Vector&
Normal::operator -=(const Vector& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}


//_____________________________________________________________________________
inline Vector&
Normal::operator*=(const FLOATING& rhs)
{
   gutils_WOUT("Methode nicht anwendbar für Normale. Nichts geschieht.");
}


//_____________________________________________________________________________
inline void
Normal::invIP()
{
  __m128d mOne = _mm_set_pd(-1, -1);
  xy = _mm_mul_pd(xy, mOne);
  zw = _mm_mul_pd(zw, mOne);
}


//_____________________________________________________________________________
inline Normal
Normal::inv() const
{
  __m128d mOne = _mm_set_pd(-1, -1);
  return Normal(_mm_mul_pd(xy, mOne), _mm_mul_pd(zw, mOne) );
}


//_____________________________________________________________________________
Vector
operator*(const FLOATING& lhs, const Vector& rhs)
{
  Vector tmp(rhs);
  tmp *= lhs;

  return tmp;
}


//_____________________________________________________________________________
inline Vector 
cross(const Vector& a, const Vector& b)
{
  __m128d ayz = _mm_shuffle_pd(a.xy, a.zw, _MM_SHUFFLE2(0, 1) );
  __m128d bzy = _mm_shuffle_pd(b.zw, b.xy, _MM_SHUFFLE2(1, 0) );
  __m128d azx = _mm_shuffle_pd(a.zw, a.xy, _MM_SHUFFLE2(0, 0) );
  __m128d bxz = _mm_shuffle_pd(b.xy, b.zw, _MM_SHUFFLE2(0, 0) );
  __m128d byx = _mm_shuffle_pd(b.xy, b.xy, _MM_SHUFFLE2(0, 1) );

  __m128d aybz_azby = _mm_mul_pd(ayz, bzy);
  __m128d azbx_axbz = _mm_mul_pd(azx, bxz);
  __m128d axby_aybx = _mm_mul_pd(a.xy, byx);

                    
  //return Vector(aybz_azby.m128d_f64[0] - aybz_azby.m128d_f64[1],
  //              azbx_axbz.m128d_f64[0] - azbx_axbz.m128d_f64[1],
  //              axby_aybx.m128d_f64[0] - axby_aybx.m128d_f64[1]);

  return Vector(_mm_hsub_pd(aybz_azby, azbx_axbz),
                _mm_hsub_pd(axby_aybx, _mm_set_pd(0,0) ) );
 
  //return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}


//_____________________________________________________________________________
inline FLOATING
angle(const Vector& a, const Vector& b)
{
  gutils_assert(a.length2() > 0. && b.length2() > 0.);

  return acos(a*b / (a.length() * b.length() ) );
}


//_____________________________________________________________________________
inline FLOATING
angle(const Normal& a, const Normal& b)
{
  gutils_assert(a.length2() > 0. && b.length2() > 0.);

  return acos(a*b);
}


//_____________________________________________________________________________
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
