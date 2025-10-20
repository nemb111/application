
#include <limits>


/********************************class HMatrix********************************/
inline HMatrix
HMatrix::Identity()
{
  FLOATING entries[16] = {1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1};
  return HMatrix(entries);
}


inline HMatrix
HMatrix::operator*(const HMatrix& rhs) const
{
  HMatrix tmp;
  matMult(rhs._entries, tmp._entries);

  return tmp;
}

inline Vector
HMatrix::operator*(const Vector& rhs) const
{
  FLOATING vec_entr[4] = {0};
  matVecMult(rhs, vec_entr);

  return Vector(vec_entr[0], vec_entr[1], vec_entr[2], vec_entr[3]);
}

inline Point
HMatrix::operator*(const Point& rhs) const
{
  FLOATING vec_entr[4] = {0};
  matVecMult(rhs, vec_entr);

  return Point(vec_entr[0], vec_entr[1], vec_entr[2], vec_entr[3]);
}

inline Normal
HMatrix::operator*(const Normal& rhs) const
{
  Vector result = operator*(dynamic_cast<const Vector&>(rhs) );
  result.w = 0;

  return Normal(result);
}

inline bool
HMatrix::operator==(const HMatrix& rhs) const
{
  bool result = true;
  for (int i=0; i<16; i++)
    result &= gutils::equals(_entries[i], rhs._entries[i]);

  return result;
}

inline const FLOATING&
HMatrix::operator[](int idx) const
{
  return _entries[idx];
}

inline FLOATING&
HMatrix::operator[](int idx)
{
  return _entries[idx];
}

inline HMatrix
HMatrix::transf(const HMatrix& mat)
{
  matMultIP(mat._entries);
  return *this;
}

inline void
HMatrix::matMult(const FLOATING mat[16], FLOATING resMat[16]/*=NULL*/) const
{
  FLOATING dotProduct = 0;

  //_entries wird nicht verändert -> beide Matrizen bleiben unverändert
  for (int i=0; i<16; i++)
  {
    for (int j=0; j<4; j++)
    {
      dotProduct += _entries[4*j + (i%4)] * mat[j + 4*(i/4)];
    }
    resMat[i] = dotProduct;
    dotProduct = 0;
  }
}

inline void 
HMatrix::matMultIP(const FLOATING mat[16])
{
  FLOATING dotProduct = 0;

  //Inplace Matrix-Multiplikation
  FLOATING tmpMat[16] = {0};
  for (int i=0; i<16; i++)
  {
    for (int j=0; j<4; j++)
    {
      dotProduct += mat[4*j + (i%4)] * _entries[j + 4*(i/4)];
    }
    tmpMat[i] = dotProduct;
    dotProduct = 0;
  }
  for (int i=0; i<16; i++)
    _entries[i] = tmpMat[i];

}

inline void
HMatrix::matVecMult(const HVector& vec, FLOATING vec_entries[4]) const
{

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      vec_entries[i] += _entries[j*4+i] * vec[j];
}

inline HMatrix
HMatrix::transpose() const
{
  HMatrix result;
  for (int i=0; i<16; i++)
    result._entries[(i%4)*4+i/4] = _entries[i];

  return result;
}

inline const HMatrix&
HMatrix::transposeIP()
{
  FLOATING tmp[16];
  for (int i=0; i<16; i++)
    tmp[(i%4)*4+i/4] = _entries[i];

  for (int i=0; i<16; i++)
    _entries[i] = tmp[i];

  return *this;
}

inline HMatrix
HMatrix::invert() const
{
  HMatrix tmp;
  if (invertMatrix(_entries, tmp._entries) )
    return tmp;
  else
  {
    gutils_EOUT("Matrix nicht invertierbar.");
    std::cin.get();
    exit(1);
  }    
}

inline const HMatrix&
HMatrix::invertIP()
{
  invertMatrix(_entries, _entries);
  return *this;
}


/************************HMatrix***********************************/
inline HMatrix::HMatrix()
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      _entries[j+4*i] = (i == j) ? (FLOATING)1 : (FLOATING)0;
}


inline HMatrix::HMatrix(const HVector& col1, const HVector& col2,
                        const HVector& col3, const HVector& col4)
{
  for (int i=0; i<4; i++)
    _entries[i] = col1[i];

  for (int i=4,j=0; i<8; i++,j++)
    _entries[i] = col2[j];

  for (int i=8,j=0; i<12; i++,j++)
    _entries[i] = col3[j];

  for (int i=12,j=0; i<16; i++,j++)
    _entries[i] = col4[j];
}


inline HMatrix::HMatrix(const FLOATING entries[16])
{
  for (int i=0; i<16; i++)
    _entries[i] = entries[i];
}


inline HMatrix::HMatrix(FLOATING n0, FLOATING n1, FLOATING n2, FLOATING n3, FLOATING n4,
                        FLOATING n5, FLOATING n6, FLOATING n7, FLOATING n8, FLOATING n9,
                        FLOATING n10, FLOATING n11, FLOATING n12, FLOATING n13, 
                        FLOATING n14, FLOATING n15)
{
  _entries[0] = n0;
  _entries[1] = n1;
  _entries[2] = n2;
  _entries[3] = n3;
  _entries[4] = n4;
  _entries[5] = n5;
  _entries[6] = n6;
  _entries[7] = n7;
  _entries[8] = n8;
  _entries[9] = n9;
  _entries[10] = n10;
  _entries[11] = n11;
  _entries[12] = n12;
  _entries[13] = n13;
  _entries[14] = n14;
  _entries[15] = n15;
}


inline bool
HMatrix::invertMatrix(FLOATING const mat[16], FLOATING inv[16]) const
{
#undef max
  // erzeuge 4x8 Matrix für LR-Zerlegung
  FLOATING tmp_mat[4][8];
  for (int i=0; i<4; i++)
  {
    //kopiere übergebene Matrix
    for (int j=0; j<4; j++)
      tmp_mat[i][j] = mat[j*4+i];
    //erzeuge Einheitsmatrix direkt rechts neben der kopierten Matrix
    for (int j=4; j<8; j++)
      tmp_mat[i][j] = (j-4 == i) ? (FLOATING)1 : (FLOATING)0;
  }

  //Berechne obere Dreiecksmatrix mit Pivotisierung
  for (int i=0; i<4-1; i++)
  {
    //Finde Pivotelement falls der aktuelle Eintrag 0 ist
    if (gutils::equals(tmp_mat[i][i], (FLOATING)0.0) )
    {
      int idx;
      FLOATING p = -std::numeric_limits<FLOATING>::max();
      for (int k=i+1; k<4; k++)
      {
        if (tmp_mat[k][i] > p)
        {
          p = tmp_mat[k][i];
          idx = k;
        }
      }
      //Vertausche die oberste Zeile mit der Zeile die das Pivotelement enthält
      switchRow(tmp_mat, i, idx);
    }
          
    //Beginne damit die Matrix auf Stufenform zu bringen (rechte DreiecsksMatrix)
    for (int l=i+1; l<4; l++)
    {
      //Wenn ein Diagonalelement 0 ist, kann die Matrix nicht invertiert werden.
      if (gutils::equals(tmp_mat[i][i], (FLOATING)0.) ) return false;
      FLOATING nullify = tmp_mat[l][i] / tmp_mat[i][i];
      for (int m=i; m<4; m++) //Umgleichung linke Matrix
        tmp_mat[l][m] -= nullify * tmp_mat[i][m];
      for (int m=0; m<4; m++) //Umgleichung rechte Matrix
        tmp_mat[l][m+4] -= nullify * tmp_mat[i][m+4];
    }
  }
 

  //Beginne damit die Matrix in reine Diagonalform zu bringen
  for (int i=3; i>0; i--)
  {
    for (int j=i-1; j>=0; j--)
    {
      FLOATING nullify = tmp_mat[j][i] / tmp_mat[i][i];
      for(int k=4; k<8; k++)
      {
        tmp_mat[j][k] -= tmp_mat[i][k] * nullify;
      }
    }
  }

  //Normalisiere die Diagonalmatrix
  for (int i=0; i<4; i++)
  {
    FLOATING normalizer = (FLOATING)1./tmp_mat[i][i];
    for (int j=4; j<8; j++)
      tmp_mat[i][j] *= normalizer;
  }

  //kopiere Matrix zurück
  for (int i=0; i<16; i++)
    inv[i] = tmp_mat[i%4][i/4+4];

  return true;
}

inline void
HMatrix::switchRow(FLOATING mat[4][8], int row1, int row2) const
{
  FLOATING tmp[8];
  for (int i=0; i<8; i++)
  {
     tmp[i] = mat[row1][i];
     mat[row1][i] = mat[row2][i];
     mat[row2][i] = tmp[i];
  }
}


/******************************TransMat***************************************/
inline TransMat::TransMat(FLOATING tx, FLOATING ty, FLOATING tz)
  : HMatrix(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  tx, ty, tz, 1)
{
}


/****************************RotMatX******************************************/
inline RotMatX::RotMatX(FLOATING phi)
  : HMatrix(1, 0, 0, 0,  0, cos(phi), -sin(phi), 0,  0, sin(phi), cos(phi), 0,
            0, 0, 0, 1), _phi(phi)
{
}


/****************************RotMatY******************************************/
inline RotMatY::RotMatY(FLOATING phi)
  : HMatrix(cos(phi), 0, sin(phi), 0,  0, 1, 0, 0,  -sin(phi), 0, cos(phi), 0,
            0, 0, 0, 1), _phi(phi)
{
}


/****************************RotMatZ******************************************/
inline RotMatZ::RotMatZ(FLOATING phi)
  : HMatrix(cos(phi), -sin(phi), 0, 0,  sin(phi), cos(phi), 0, 0,  0, 0, 1, 0,
            0, 0, 0, 1), _phi(phi)
{
}


/****************************RotMat******************************************/
inline RotMat::RotMat(const Vector& raxis, FLOATING phi)
  : HMatrix(), _phi(phi), _raxis(raxis)
{
  gutils_assert( _raxis != Vector(0,0,0) );

  _raxis.normIP();
  FLOATING s = sin(_phi);
  FLOATING c = cos(_phi);

  (*this)[0] = _raxis.x * _raxis.x + ((FLOATING)1. - _raxis.x * _raxis.x) * c;
  (*this)[1] = _raxis.x * _raxis.y * ((FLOATING)1. - c) - _raxis.z * s;
  (*this)[2] = _raxis.x * _raxis.z * ((FLOATING)1. - c) + _raxis.y * s;
  (*this)[3] = 0;

  (*this)[4] = _raxis.x * _raxis.y * ((FLOATING)1. - c) + _raxis.z * s;
  (*this)[5] = _raxis.y * _raxis.y + ((FLOATING)1. - _raxis.y * _raxis.y) * c;
  (*this)[6] = _raxis.y * _raxis.z * ((FLOATING)1. - c) - _raxis.x * s;
  (*this)[7] = 0;

  (*this)[8] = _raxis.x * _raxis.z * ((FLOATING)1. - c) - _raxis.y * s;
  (*this)[9] = _raxis.y * _raxis.z * ((FLOATING)1. - c) + _raxis.x * s;
  (*this)[10] = _raxis.z * _raxis.z + ((FLOATING)1. - _raxis.z * _raxis.z) * c;
  (*this)[11] = 0;

  (*this)[12] = 0;
  (*this)[13] = 0;
  (*this)[14] = 0;
  (*this)[15] = 1;
}


/****************************ScaleMat*****************************************/
inline ScaleMat::ScaleMat(FLOATING sx, FLOATING sy, FLOATING sz)
  : HMatrix(sx, 0, 0, 0,  0, sy, 0, 0,  0, 0, sz, 0,  0, 0, 0, 1)
{
}


/****************************ReflMatX*****************************************/
inline ReflMatX::ReflMatX()
  : HMatrix(-1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1)
{
}


/****************************ReflMatY*****************************************/
inline ReflMatY::ReflMatY()
  : HMatrix(1, 0, 0, 0,  0, -1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1)
{
}


/****************************ReflMatZ*****************************************/
inline ReflMatZ::ReflMatZ()
  : HMatrix(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, -1, 0,  0, 0, 0, 1)
{
}