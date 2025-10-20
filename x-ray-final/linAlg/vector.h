/*
 * vector.h
 *
 *  Created on: 12.11.2012
 *  Author: Benedikt Mendorf
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#pragma warning( disable : 4723 )

#include <string>


/*Forward declarations*/
class HVector;
class Point;
class Vector;
class Normal;


class HVector
{
  public:

    FLOATING x;
    FLOATING y;
    FLOATING z;
    FLOATING w;

    inline virtual ~HVector(){}

    inline const FLOATING& operator[](size_t idx) const;
    inline const FLOATING& operator[](const std::string& str) const;
    inline FLOATING& operator[](size_t idx);
    inline FLOATING& operator[](const std::string& str);
    inline bool operator==(const HVector& rhs) const;
    inline bool operator!=(const HVector& rhs) const;

    inline FLOATING length() const;
    inline FLOATING length2() const;

  protected:

   


    inline HVector(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
     : x(x), y(y), z(z), w(w)
    {}

  private:

  friend class HMatrix;
};


class Point : public HVector
{
  public:
    inline Point() : HVector(0, 0, 0, 1){}
    inline Point(FLOATING x, FLOATING y, FLOATING z)
     : HVector(x, y, z, 1)
    {}
    inline virtual ~Point(){}

    inline Point operator+(const Vector& rhs) const;
    inline Point& operator+=(const Vector& rhs);
    inline Vector operator-(const Point& rhs) const;
    inline Point operator*(const FLOATING& rhs) const;
    inline Point& operator*=(const FLOATING& rhs);

    // Diese Methode erlaubt das Addieren von Punkten und sollte nur zur
    // Bestimmung des Flächenschwerpunkts benutzt werden.
    inline Point baryAdd(const Point& rhs) const;

    inline Point normW() const;
    inline const Point&  normWIP();

  protected:
    inline Point(FLOATING x, FLOATING y, FLOATING z, FLOATING w)
      : HVector(x, y, z, w)
    {}

  private:


  friend class HMatrix;
};

inline Point operator+(const Vector& lhs, const Point& rhs);
inline Point operator*(const FLOATING& lhs, const Point& rhs);

class Vector : public HVector
{
  public:
    inline Vector() : HVector(0, 0, 0, 0){}
    inline Vector(FLOATING x, FLOATING y, FLOATING z)
     : HVector(x, y, z, 0)
    {}
    inline virtual ~Vector(){}

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
    {}

  private:

  friend class Point;
  friend class HMatrix;
};

inline Vector operator*(const FLOATING& lhs, const Vector& rhs);
inline Vector cross(const Vector& a, const Vector& b);
inline FLOATING angle(const Vector& a, const Vector& b);


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
    gutils_assert(gutils::equals(rhs.length(), (FLOATING)1.) );
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
  inline Vector& operator -=(const Vector& rhs);
  inline Vector& operator*=(const FLOATING& rhs);

  inline void invIP();
  inline Normal inv() const;

};

inline FLOATING angle(const Normal& a, const Normal& b);




// Erzeugt eine rechtshändische Orthonormalebasis aus Vektor e1 und e2.
// e3 wird automatisch erzeugt.
inline void orthNormBasis(Vector& e1, Vector& e2, Vector& e3);

#include "vector.inl"

#endif /* VECTOR_H_ */