#pragma once

#ifndef VECTOR_SSE_H_
#define VECTOR_SSE_H_


#include <emmintrin.h>
#include <pmmintrin.h>


#include <string>


/*Forward declarations*/
//class HVector;
//class Point;
class Vector;
//class Normal;

//_____________________________________________________________________________
__declspec( align(16) ) class HVector
{
public:

#if PRECISION == 1
  union __declspec(align(16) )
  {
    struct {float x, y, z, w;};
    __m128 xyzw;
  };
#else
  union __declspec(align(16) )
  {
    __m128d xy;
    struct  {double x, y;};
  };
  union __declspec(align(16) )
  {
    __m128d zw;
    struct {double z, w;};
  };
#endif

  inline ~HVector(){}

  inline const FLOATING& operator[](size_t idx) const;
  inline const FLOATING& operator[](const std::string& str) const;
  inline FLOATING& operator[](size_t idx);
  inline FLOATING& operator[](const std::string& str);
  inline bool operator==(const HVector& rhs) const;
  inline bool operator!=(const HVector& rhs) const;

  inline FLOATING length() const;
  inline FLOATING length2() const;

protected:

#if PRECISION == 1
  inline HVector(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
    : xyzw(_mm_set_ps(w,z,y,x) )
  {
    gutils_assert(&xyzw.m128_f32[0] == &this->x);
    gutils_assert(&xyzw.m128_f32[1] == &this->y);
    gutils_assert(&xyzw.m128_f32[2] == &this->z);
    gutils_assert(&xyzw.m128_f32[3] == &this->w);
  }
  inline HVector(const __m128& xyzwRHS)
    : xyzw(xyzwRHS) {}
#else
  inline HVector(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
    : xy(_mm_set_pd(y, x) ), zw(_mm_set_pd(w, z) )
  {
    gutils_assert(&xy.m128d_f64[0] == &this->x);
    gutils_assert(&xy.m128d_f64[1] == &this->y);
    gutils_assert(&zw.m128d_f64[0] == &this->z);
    gutils_assert(&zw.m128d_f64[1] == &this->w);
  }
  inline HVector(const __m128d& xyRHS, const __m128d& zwRHS)
    : xy(xyRHS), zw(zwRHS) {}
#endif
 
private:

  friend class HMatrix;
};


//_____________________________________________________________________________
class Point : public HVector
{
public:

  inline Point() : HVector(0, 0, 0, 1){}
  inline Point(FLOATING x, FLOATING y, FLOATING z)
    : HVector(x, y, z, 1)
  {}
  inline ~Point(){}

  inline Point operator+(const Vector& rhs) const;
  inline Point& operator+=(const Vector& rhs);
  inline Vector operator-(const Point& rhs) const;
  inline Point operator*(const FLOATING& rhs) const;
  inline Point& operator*=(const FLOATING& rhs);

  // Diese Methode erlaubt das Addieren von Punkten und sollte nur zur
  // Bestimmung des Flächenschwerpunkts benutzt werden.
  inline Point baryAdd(const Point& rhs) const;

  inline FLOATING length() const;
  inline FLOATING length2() const;
  inline Point normW() const;
  inline const Point&  normWIP();

protected:
  inline Point(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
    : HVector(x, y, z, w)
  {}
#if PRECISION == 1
  inline Point(const __m128& xyzwRHS)
    : HVector(xyzwRHS) {}
#else
  inline Point(const __m128d& xyRHS, const __m128d& zwRHS)
    : HVector(xyRHS, zwRHS)
  {}
#endif // PRECISION == 1

private:

friend class HMatrix;
};

inline Point operator+(const Vector& lhs, const Point& rhs);
inline Point operator*(const FLOATING& lhs, const Point& rhs);


//_____________________________________________________________________________
class Vector : public HVector
{
  public:
    inline Vector() : HVector(0, 0, 0, 0){}
    inline Vector(FLOATING x, FLOATING y, FLOATING z)
     : HVector(x, y, z, 0)
    {}
    inline ~Vector(){}

    inline Vector operator+(const Vector& rhs) const;
    inline Vector& operator+=(const Vector& rhs);
    inline Vector operator-(const Vector& rhs) const;
    inline Vector& operator-=(const Vector& rhs);
    inline Vector operator*(const FLOATING& rhs) const;
    inline FLOATING operator*(const Vector& rhs) const;
    inline Vector& operator*=(const FLOATING& rhs);

    inline Vector norm() const;
    inline const Vector& normIP();

  protected:
    inline Vector(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
      : HVector (x, y, z, w)
    {
      gutils_assert(gutils::equals<FLOATING>(w, (FLOATING)0.) );
    }
#if PRECISION == 1
    inline Vector(const __m128& xyzwRHS)
      : HVector(xyzwRHS)
    {
      gutils_assert(gutils::equals<FLOATING>(w, (FLOATING)0.) );
    }
#else
    inline Vector(const __m128d& xyRHS, const __m128d& zwRHS)
      : HVector(xyRHS, zwRHS)
    {
      gutils_assert(gutils::equals<FLOATING>(w, (FLOATING)0.) );
    }
#endif //PRECISION == 1

  private:

  friend class Point;
  friend class HMatrix;
  friend inline Vector cross(const Vector& a, const Vector& b);
};

inline Vector operator*(const FLOATING& lhs, const Vector& rhs);
inline Vector cross(const Vector& a, const Vector& b);
inline FLOATING angle(const Vector& a, const Vector& b);


//_____________________________________________________________________________
class Normal : public Vector
{
public:
  Normal() : Vector(0,0,1){}
  Normal(FLOATING x, FLOATING y, FLOATING z)
    : Vector(x, y, z)
  {
    this->normIP();
  }
  Normal(const Normal& rhs)
  {
    gutils_assert(gutils::equals<FLOATING>(rhs.length(), 
                                           static_cast<FLOATING>(1.f) ) );
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
  }
  Normal(const Vector& vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;

    this->normIP();
  }

  inline const Normal& operator=(const Vector& vec);

  inline FLOATING& operator[](int idx);
  inline FLOATING& operator[](const std::string& str);
  inline Vector& operator+=(const Vector& rhs);
  inline Vector& operator-=(const Vector& rhs);
  inline Vector& operator*=(const FLOATING& rhs);

  inline void invIP();
  inline Normal inv() const;

protected:
#if PRECISION == 1
  inline Normal(const __m128 xyzwRHS)
    : Vector(xyzwRHS) {}
#else
  inline Normal(const __m128d& xyRHS, const __m128d& zwRHS)
    : Vector(xyRHS, zwRHS) {}

#endif //PRECISION == 1

};

inline FLOATING angle(const Normal& a, const Normal& b);




// Erzeugt eine rechtshändische Orthonormalebasis aus Vektor e1 und e2.
// e3 wird automatisch erzeugt.
inline void orthNormBasis(Vector& e1, Vector& e2, Vector& e3);


#if PRECISION == 1
#include "vector_sse_sp.inl"
#else
#include "vector_sse_dp.inl"
#endif

#endif //VECTOR_SSE_H_