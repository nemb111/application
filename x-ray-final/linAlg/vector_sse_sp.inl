
#include <iostream>


//_____________________________________________________________________________
inline const FLOATING&
HVector::operator[](size_t idx) const
{
  gutils_assert(x >= 0 && x < 4);

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
  gutils_assert(x >= 0 && x < 4);

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
  __m128 epsilon     = _mm_set_ps1(gutils::EPSILON);
  __m128 diffXYZW    = _mm_sub_ps(xyzw, rhs.xyzw);
  __m128 negDiffXYZW = _mm_sub_ps(rhs.xyzw, xyzw);
  __m128 absDiffXYZW = _mm_max_ps(diffXYZW, negDiffXYZW);
  __m128 resVec = _mm_cmplt_ps(absDiffXYZW, epsilon);
  int res = _mm_movemask_ps(resVec);

  return res == 0xf;
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
  return sqrt( length2() );
}


//_____________________________________________________________________________
inline FLOATING
HVector::length2() const
{
  __m128 mulVec = _mm_mul_ps(xyzw, xyzw);
  __m128 res    = _mm_hadd_ps(mulVec, mulVec);
  res    = _mm_hadd_ps(res, res);
  return res.m128_f32[0];
}


/***********************************Point*************************************/
inline Point
Point::operator+(const Vector& rhs) const
{
  return _mm_add_ps(xyzw, rhs.xyzw);
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
  float wInv = 1.f/w;
  float wRHSInv = 1.f/rhs.w;

  __m128 wVec = _mm_set_ps1(wInv);
  __m128 wRHSVec = _mm_set_ps1(wRHSInv);

  return _mm_sub_ps(_mm_mul_ps(xyzw, wVec), _mm_mul_ps(rhs.xyzw, wRHSVec) );
}


//_____________________________________________________________________________
Point
Point::operator*(const FLOATING& rhs) const
{
  __m128 mMul = _mm_set_ps(w * rhs - 1, 0, 0, 0);
  return _mm_sub_ps(_mm_mul_ps(xyzw, _mm_set1_ps(rhs) ), mMul);
}


//_____________________________________________________________________________
Point&
Point::operator*=(const FLOATING& rhs)
{
  *this = *this * rhs;

  return *this;
}


//_____________________________________________________________________________
Point
Point::baryAdd(const Point& rhs) const
{
  return _mm_add_ps(xyzw, rhs.xyzw);
}

//_____________________________________________________________________________
inline FLOATING 
Point::length() const
{
  return sqrt(length2() );
}


//_____________________________________________________________________________
inline FLOATING
Point::length2() const
{
  gutils_assert(! gutils::equals(w, 0.f) );

  __m128 wInv = _mm_set1_ps(1.f/w);
  __m128 res  = _mm_mul_ps(xyzw, wInv);
         res  = _mm_mul_ps(res, res);
         res  = _mm_hadd_ps(res, res);
         res  = _mm_hadd_ps(res, res);
  return res.m128_f32[0] - 1;
}


//_____________________________________________________________________________
inline Point
Point::normW() const
{
  gutils_assert(!gutils::equals(0.f, w) );

  __m128 wInv = _mm_set1_ps(1.f/w);
  return _mm_mul_ps(xyzw, wInv);
}


//_____________________________________________________________________________
inline const Point&
Point::normWIP()
{
  gutils_assert(!gutils::equals(0.f, w) );

  __m128 wInv = _mm_set1_ps(1.f/w);
  xyzw = _mm_mul_ps(xyzw, wInv);

  return *this;
}


//_____________________________________________________________________________
Point
operator+(const Vector& lhs, const Point& rhs)
{
  return rhs + lhs;
}


//_____________________________________________________________________________
Point
operator*(const FLOATING& lhs, const Point& rhs)
{
  return rhs * lhs;
}


/********************************Vector***************************************/
Vector
Vector::operator+(const Vector& rhs) const
{
  gutils_assert(gutils::equals(0.f, w) );
  gutils_assert(gutils::equals(0.f, rhs.w) );

  return _mm_add_ps(xyzw, rhs.xyzw);
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
  gutils_assert(gutils::equals(0.f, w) );
  gutils_assert(gutils::equals(0.f, rhs.w) );

  return _mm_sub_ps(xyzw, rhs.xyzw);
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
  gutils_assert(gutils::equals(0.f, w) );

  return _mm_mul_ps(xyzw, _mm_set1_ps(rhs) );
}


//_____________________________________________________________________________
FLOATING
Vector::operator*(const Vector& rhs) const
{
  __m128 res = _mm_mul_ps(xyzw, rhs.xyzw);
         res = _mm_hadd_ps(res, res);
         res = _mm_hadd_ps(res, res);
  return res.m128_f32[0];
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
  __m128 lengthInv =
    _mm_set1_ps((gutils::equals(length, (FLOATING)0.) ) ? 1 : (FLOATING)1./length);

  return _mm_mul_ps(xyzw, lengthInv);
}


//_____________________________________________________________________________
inline const Vector&
Vector::normIP()
{
  FLOATING length = this->length();
  __m128 lengthInv =
    _mm_set1_ps((gutils::equals(length, (FLOATING)0.) ) ? 1 : (FLOATING)1./length);

  xyzw = _mm_mul_ps(xyzw, lengthInv);

  return *this;
}


//_____________________________________________________________________________
inline const Normal& 
Normal::operator=(const Vector& vec)
{
  gutils_assert(gutils::equals(vec.w, (FLOATING)0.) );

  xyzw = vec.xyzw;
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
  xyzw = _mm_mul_ps(xyzw, _mm_set1_ps(-1) );
}


//_____________________________________________________________________________
inline Normal
Normal::inv() const
{
  return _mm_mul_ps(xyzw, _mm_set1_ps(-1) );
}


//_____________________________________________________________________________
Vector
operator*(const FLOATING& lhs, const Vector& rhs)
{
  return rhs * lhs;
}


//_____________________________________________________________________________
inline Vector 
cross(const Vector& a, const Vector& b)
{
  return 
    _mm_sub_ps(
      _mm_mul_ps(
        _mm_shuffle_ps(a.xyzw, a.xyzw, _MM_SHUFFLE(3, 0, 2, 1) ), 
        _mm_shuffle_ps(b.xyzw, b.xyzw, _MM_SHUFFLE(3, 1, 0, 2) ) ),
      _mm_mul_ps(
        _mm_shuffle_ps(a.xyzw, a.xyzw, _MM_SHUFFLE(3, 1, 0, 2) ), 
        _mm_shuffle_ps(b.xyzw, b.xyzw, _MM_SHUFFLE(3, 0, 2, 1) ) ) );
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