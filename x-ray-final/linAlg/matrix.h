/* Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   - Dem Buch "Ray tracing from ground up" / Seite 397 - 443
   - Dem Buch "Physically based rendering" / Seite 74 - 84
*/





#ifndef HMATRIX_H_
#define HMATRIX_H_

#include "vector.h"

// Die Homogene Matrix hat die Dimension 4x4 und wird Zeilenweise gefüllt.
// D.h. Die erste Zeile wird von 0-3 indexiert und mit den Werten n0-n3 gefüllt.
class HMatrix
{
public:

  //erzeugt eine Identitätsmatrix
  static inline HMatrix Identity();

  HMatrix();
  HMatrix(const HVector& col1, const HVector& col2,
                 const HVector& col3, const HVector& col4);
  HMatrix(const FLOATING entries[16]);
  HMatrix(FLOATING n0, FLOATING n1, FLOATING n2, FLOATING n3, FLOATING n4, 
          FLOATING n5, FLOATING n6, FLOATING n7, FLOATING n8, FLOATING n9,
          FLOATING n10, FLOATING n11, FLOATING n12, FLOATING n13, 
          FLOATING n14, FLOATING n15);
  virtual ~HMatrix(){};

  //Matrixmultiplikation, die beide Matrizen unverändert lässt
  inline HMatrix operator*(const HMatrix& rhs) const;
  inline Vector  operator*(const Vector& rhs) const;
  inline Point   operator*(const Point& rhs) const;
  inline Normal  operator*(const Normal& rhs) const;
  inline bool operator==(const HMatrix& rhs) const;
  inline const FLOATING& operator[](int idx) const;
  inline FLOATING& operator[](int idx);
  
  //Transformation der Matrix, d.h. Matrixmultiplikation die in-place
  //durchgeführt wird
  inline HMatrix transf(const HMatrix& mat);
  //Die transponierte Matrix wird zurückgegeben
  inline HMatrix transpose() const;
  //Das aktuelle Objekt wird transponiert(in-place)
  inline const HMatrix& transposeIP();
  //Die invertierte Matrix wird zurückgegeben.
  inline HMatrix invert() const;
  //Das aktuelle Objekt wird transponiert(in-place).
  inline const HMatrix& invertIP();
  

protected:

  inline void matMult(const FLOATING mat[16], FLOATING resMat[16]=NULL) const;
  inline void matMultIP(const FLOATING mat[16]);
  inline void matVecMult(const HVector& vec, FLOATING vec_entries[4]) const;
         bool invertMatrix(FLOATING const mat[16], FLOATING inv[16]) const;
         void switchRow(FLOATING mat[4][8], int row1, int row2) const;

private:
  FLOATING _entries[16];



/* Frienddeklarationen */
#ifdef _DEBUG
 friend void PRTestMatrix_HMatrix_invertierung();
#endif//_Debug
};


// Translationsmatrix, die um Komponenten x, y und z verschiebt
class TransMat : public HMatrix
{
public:
  TransMat(FLOATING tx, FLOATING ty, FLOATING tz);
  virtual ~TransMat(){}
};

// Rotationsmatrix, die um Winkel phi um die x-Achse rotiert
class RotMatX : public HMatrix
{
public:
  RotMatX(FLOATING phi);
  virtual ~RotMatX(){}

protected:
  FLOATING _phi;

private:

};

// Rotationsmatrix, die um Winkel phi um die y-Achse rotiert
class RotMatY : public HMatrix
{
public:
  RotMatY(FLOATING phi);
  virtual ~RotMatY(){}

protected:
  FLOATING _phi;

private:
};

// Rotationsmatrix, die um Winkel phi um die z-Achse rotiert
class RotMatZ : public HMatrix
{
public:
  RotMatZ(FLOATING phi);
  virtual ~RotMatZ(){}

protected:
  FLOATING _phi;

private:
};

// Rotationsmatrix, die um beliebige Drehachse rotiert
class RotMat : public HMatrix
{
public:
  RotMat(const Vector& raxis, FLOATING phi);
  virtual ~RotMat(){}
protected:
  FLOATING _phi;
  Vector _raxis;

private:

};


// Skalierungsmatrix, die ein Shape entlang der x, y und z Achse skaliert
class ScaleMat : public HMatrix
{
public:
  ScaleMat(FLOATING sx, FLOATING sy, FLOATING sz);
  virtual ~ScaleMat(){}
};


// Reflektionsmatrix, die an der y-z-Achse reflektiert
class ReflMatX : public HMatrix
{
public:
  ReflMatX();
  virtual ~ReflMatX(){}
};


// Reflektionsmatrix, die an der x-z-Achse reflektiert
class ReflMatY : public HMatrix
{
public:
  ReflMatY();
  virtual ~ReflMatY(){}
};


// Reflektionsmatrix, die an der x-y-Achse reflektiert
class ReflMatZ : public HMatrix
{
public:
  ReflMatZ();
  virtual ~ReflMatZ(){}
};




#include "matrix.inl"

#endif //HMATRIX_H_