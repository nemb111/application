#include <limits>

/******************************************************************************
******************************* PRECISION 1 ***********************************
******************************************************************************/

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


//_____________________________________________________________________________
inline HMatrix
HMatrix::operator*(const HMatrix& rhs) const
{
  HMatrix tmp;
  matMult(rhs._entries, tmp._entries);

  return tmp;
}


//_____________________________________________________________________________
inline Vector
HMatrix::operator*(const Vector& rhs) const
{
  __m128 vec_entr;

  matVecMult(rhs, vec_entr);

  gutils_assert(gutils::equals<FLOATING>(vec_entr.m128_f32[3], 0) );
  return Vector(vec_entr);
}


//_____________________________________________________________________________
inline Point
HMatrix::operator*(const Point& rhs) const
{
  __m128 vec_entr;

  matVecMult(rhs, vec_entr);

  return Point(vec_entr);
}


//_____________________________________________________________________________
inline Normal
HMatrix::operator*(const Normal& rhs) const
{
  Vector result = operator*(dynamic_cast<const Vector&>(rhs) );

  gutils_assert(gutils::equals<FLOATING>(result.w, 0) );
  return Normal(result);
}


//_____________________________________________________________________________
inline bool
HMatrix::operator==(const HMatrix& rhs) const
{
  return 
    (HVector(_entries[0]) == HVector(rhs._entries[0]) &&
     HVector(_entries[1]) == HVector(rhs._entries[1]) &&
     HVector(_entries[2]) == HVector(rhs._entries[2]) &&
     HVector(_entries[3]) == HVector(rhs._entries[3]) );
}


//_____________________________________________________________________________
inline const FLOATING&
HMatrix::operator[](int idx) const
{
  gutils_assert(idx >= 0 && idx < 16);
  return _entries[idx/4].m128_f32[idx%4];
}


//_____________________________________________________________________________
inline FLOATING&
HMatrix::operator[](int idx)
{
  gutils_assert(idx >= 0 && idx < 16);
  return _entries[idx/4].m128_f32[idx%4];
}


//_____________________________________________________________________________
inline HMatrix
HMatrix::transf(const HMatrix& mat)
{
  matMultIP(mat._entries);
  return *this;
}


//_____________________________________________________________________________
inline void
HMatrix::matMult(const __m128 mat[4], __m128 resMat[4]/*=NULL*/) const
{
  __m128 a_line, b_line, r_line;
  for (int i=0; i<4; i++)
  {
    // unroll the first step of the loop to avoid having to initialize r_line to zero
    a_line = _entries[0];         // a_line = vec4(column(a, 0))
    b_line = _mm_set1_ps(mat[i].m128_f32[0]);      // b_line = vec4(b[i][0])
    r_line = _mm_mul_ps(a_line, b_line); // r_line = a_line * b_line
    for (int j=1; j<4; j++)
    {
      a_line = _entries[j];     // a_line = vec4(column(a, j))
      b_line = _mm_set1_ps(mat[i].m128_f32[j]);  // b_line = vec4(b[i][j])
                                     // r_line += a_line * b_line
      r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
    }
    resMat[i] = r_line;     // r[i] = r_line
  }
}


//_____________________________________________________________________________
inline void 
HMatrix::matMultIP(const __m128 mat[4])
{
  __m128 resMat[4];
  __m128 a_line, b_line, r_line;
  for (int i=0; i<4; i++)
  {
    // unroll the first step of the loop to avoid having to initialize r_line to zero
    a_line = mat[0];         // a_line = vec4(column(a, 0))
    b_line = _mm_set1_ps(_entries[i].m128_f32[0]);      // b_line = vec4(b[i][0])
    r_line = _mm_mul_ps(a_line, b_line); // r_line = a_line * b_line
    for (int j=1; j<4; j++)
    {
      a_line = mat[j];     // a_line = vec4(column(a, j))
      b_line = _mm_set1_ps(_entries[i].m128_f32[j]);  // b_line = vec4(b[i][j])
                                     // r_line += a_line * b_line
      r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
    }
    resMat[i] = r_line;     // r[i] = r_line
  }


  for (int i=0; i<4; i++)
    _entries[i] = resMat[i];
}


//_____________________________________________________________________________
inline void
HMatrix::matVecMult(const HVector& vec, __m128& vec_entries) const
{
  __m128 col1, col2, col3, col4;
  __m128 xVec = _mm_set1_ps(vec.x);
  __m128 yVec = _mm_set1_ps(vec.y);
  __m128 zVec = _mm_set1_ps(vec.z);
  __m128 wVec = _mm_set1_ps(vec.w);

  col1 = _mm_mul_ps(_entries[0], xVec);
  col2 = _mm_mul_ps(_entries[1], yVec);
  col3 = _mm_mul_ps(_entries[2], zVec);
  col4 = _mm_mul_ps(_entries[3], wVec);

  vec_entries = _mm_add_ps(_mm_add_ps(col1, col2), _mm_add_ps(col3, col4) );
}


//_____________________________________________________________________________
inline HMatrix
HMatrix::transpose() const
{
  __m128 entries[4];
  for (int i=0; i<4; i++)
    entries[i] = _entries[i];

  _MM_TRANSPOSE4_PS(entries[0], entries[1], entries[2], entries[3]);

  return entries;
}


//_____________________________________________________________________________
inline const HMatrix&
HMatrix::transposeIP()
{
  _MM_TRANSPOSE4_PS(_entries[0], _entries[1], _entries[2], _entries[3]);

  return *this;
}


//_____________________________________________________________________________
inline HMatrix
HMatrix::invert() const
{
  FLOATING entries[16], resEntries[16];

  for (int i=0; i<16; i++)
    entries[i] = (*this)[i];

  if (invertMatrix(entries, resEntries) )
    return resEntries;
  else
  {
    gutils_EOUT("Matrix nicht invertierbar.");
    std::cin.get();
    exit(1);
  }    
}


//_____________________________________________________________________________
inline const HMatrix&
HMatrix::invertIP()
{
  FLOATING entries[16], resEntries[16];

  for (int i=0; i<16; i++)
    entries[i] = (*this)[i];

  invertMatrix(entries, resEntries);

  for (int i=0; i<16; i++)
    (*this)[i] = resEntries[i];


  return *this;
}


//_____________________________________________________________________________
inline HMatrix::HMatrix(const __m128 entries[4])
{
  for (int i=0; i<4; i++)
    _entries[i] = entries[i];
}


/************************HMatrix***********************************/
inline HMatrix::HMatrix()
{
  _entries[0] = _mm_set_ps(0, 0, 0, 1);
  _entries[1] = _mm_set_ps(0, 0, 1, 0);
  _entries[2] = _mm_set_ps(0, 1, 0, 0);
  _entries[3] = _mm_set_ps(1, 0, 0, 0);
}


//_____________________________________________________________________________
inline HMatrix::HMatrix(const HVector& col1, const HVector& col2,
                        const HVector& col3, const HVector& col4)
{
  _entries[0] = col1.xyzw;
  _entries[1] = col2.xyzw;
  _entries[2] = col3.xyzw;
  _entries[3] = col4.xyzw;
}


//_____________________________________________________________________________
inline HMatrix::HMatrix(const FLOATING entries[16])
{
  for (int i=0, j=0; i<4; i++, j+=4)
    _entries[i] = _mm_set_ps(entries[j+3], entries[j+2],
                             entries[j+1], entries[j]);
}


//_____________________________________________________________________________
inline HMatrix::HMatrix(FLOATING n0, FLOATING n1, FLOATING n2, FLOATING n3, FLOATING n4,
                        FLOATING n5, FLOATING n6, FLOATING n7, FLOATING n8, FLOATING n9,
                        FLOATING n10, FLOATING n11, FLOATING n12, FLOATING n13, 
                        FLOATING n14, FLOATING n15)
{
  _entries[0] = _mm_set_ps(n3, n2, n1, n0);
  _entries[1] = _mm_set_ps(n7, n6, n5, n4);
  _entries[2] = _mm_set_ps(n11, n10, n9, n8);
  _entries[3] = _mm_set_ps(n15, n14, n13, n12);
}


//_____________________________________________________________________________
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
