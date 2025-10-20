
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#pragma once


#ifndef SPECTRUM_H_
#define SPECTRUM_H_

#define NOMINMAX

#include "float.h"
#include "x-ray.h"

class RGBSpectrum;
class RGBASpectrum;
typedef RGBSpectrum Spectrum;
typedef unsigned int SPECTRUM_RGB;
typedef unsigned int SPECTRUM_RGBA;


// Spectrum Utility Declarations
inline void XYZToRGB(const float xyz[3], float rgb[3]) {
    rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
    rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
    rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
}


inline void RGBToXYZ(const float rgb[3], float xyz[3]) {
    xyz[0] = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
    xyz[1] = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
    xyz[2] = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];
}


enum SpectrumType { SPECTRUM_REFLECTANCE, SPECTRUM_ILLUMINANT };


/***************************CoefficientSpectrum*******************************/
// Spectrum Declarations
template <int nSamples> class CoefficientSpectrum {
public:
    // CoefficientSpectrum Public Methods
    CoefficientSpectrum(float v = 0.f) {
        numSamples = nSamples;
        for (int i = 0; i < nSamples; ++i)
            c[i] = v;
        assert(!HasNaNs());
    }
#ifdef _DEBUG
    CoefficientSpectrum(const CoefficientSpectrum &s) {
        assert(!s.HasNaNs());
        numSamples = s.numSamples;
        for (int i = 0; i < nSamples; ++i)
            c[i] = s.c[i];
    }
    
    CoefficientSpectrum &operator=(const CoefficientSpectrum &s) {
        assert(!s.HasNaNs());
        numSamples = s.numSamples;
        for (int i = 0; i < nSamples; ++i)
            c[i] = s.c[i];
        return *this;
    }
#endif // DEBUG
    void Print(FILE *f) const {
        fprintf(f, "[ ");
        for (int i = 0; i < nSamples; ++i) {
            fprintf(f, "%f", c[i]);
            if (i != nSamples-1) fprintf(f, ", ");
        }
        fprintf(f, "]");
    }
    float& operator[](int idx) {
      assert(idx >= 0 && idx < numSamples);
      return c[idx];
    }
    const float& operator[](int idx) const {
      assert(idx >= 0 && idx < this->numSamples);
      return c[idx];
    }
    CoefficientSpectrum &operator+=(const CoefficientSpectrum &s2) {
        assert(!s2.HasNaNs());
        for (int i = 0; i < nSamples; ++i)
            c[i] += s2.c[i];
        return *this;
    }
    CoefficientSpectrum operator+(const CoefficientSpectrum &s2) const {
        assert(!s2.HasNaNs());
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] += s2.c[i];
        return ret;
    }
    CoefficientSpectrum operator-(const CoefficientSpectrum &s2) const {
        assert(!s2.HasNaNs());
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] -= s2.c[i];
        return ret;
    }
    CoefficientSpectrum operator/(const CoefficientSpectrum &s2) const {
        assert(!s2.HasNaNs());
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] /= s2.c[i];
        return ret;
    }
    CoefficientSpectrum operator*(const CoefficientSpectrum &sp) const {
        assert(!sp.HasNaNs());
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] *= sp.c[i];
        return ret;
    }
    CoefficientSpectrum &operator*=(const CoefficientSpectrum &sp) {
        assert(!sp.HasNaNs());
        for (int i = 0; i < nSamples; ++i)
            c[i] *= sp.c[i];
        return *this;
    }
    CoefficientSpectrum operator*(float a) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] *= a;
        assert(!ret.HasNaNs());
        return ret;
    }
    CoefficientSpectrum &operator*=(float a) {
        for (int i = 0; i < nSamples; ++i)
            c[i] *= a;
        assert(!HasNaNs());
        return *this;
    }
    friend inline
    CoefficientSpectrum operator*(float a, const CoefficientSpectrum &s) {
        assert(!_isnan(a) && !s.HasNaNs());
        return s * a;
    }
    CoefficientSpectrum operator/(float a) const {
        assert(!_isnan(a));
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] /= a;
        assert(!ret.HasNaNs());
        return ret;
    }
    CoefficientSpectrum &operator/=(float a) {
        assert(!_isnan(a));
        for (int i = 0; i < nSamples; ++i)
            c[i] /= a;
        return *this;
    }
    bool operator==(const CoefficientSpectrum &sp) const {
        for (int i = 0; i < nSamples; ++i)
            if (c[i] != sp.c[i]) return false;
        return true;
    }
    bool operator!=(const CoefficientSpectrum &sp) const {
        return !(*this == sp);
    }
    bool IsBlack() const {
        for (int i = 0; i < nSamples; ++i)
            if (c[i] != 0.) return false;
        return true;
    }
    friend CoefficientSpectrum Sqrt(const CoefficientSpectrum &s) {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] = sqrtf(s.c[i]);
        assert(!ret.HasNaNs());
        return ret;
    }
    template <int n> friend inline CoefficientSpectrum<n> Pow(const CoefficientSpectrum<n> &s, float e);

    CoefficientSpectrum operator-() const {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] = -c[i];
        return ret;
    }
    friend CoefficientSpectrum Exp(const CoefficientSpectrum &s) {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] = expf(s.c[i]);
        assert(!ret.HasNaNs());
        return ret;
    }
    CoefficientSpectrum Clamp(float low = 0, float high = INFINITY) const {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] = ::Clamp(c[i], low, high);
        assert(!ret.HasNaNs());
        return ret;
    }
    float magnitude() const {
      float tmp = 0;
      for (int i = 0; i < nSamples; ++i)
        tmp += c[i];

      return tmp;
    }
    float maxCoeff() const {
      float tmp = -1000000;
      for (int i = 0; i < nSamples; ++i)
        tmp = (c[i] > tmp) ? c[i] : tmp;

      return tmp;
    }
    bool HasNaNs() const {
        for (int i = 0; i < nSamples; ++i)
            if (_isnan(c[i])) return true;
        return false;
    }
    bool Write(FILE *f) const {
        for (int i = 0; i < nSamples; ++i)
            if (fprintf(f, "%f ", c[i]) < 0) return false;
        return true;
    }
    bool Read(FILE *f) {
        for (int i = 0; i < nSamples; ++i)
            if (fscanf(f, "%f ", &c[i]) != 1) return false;
        return true;
    }

public:
    int numSamples;

protected:
    // CoefficientSpectrum Protected Data
    float c[nSamples];
    
};


/********************************RGBSpectrum**********************************/

#define BLACK     0x000000
#define WHITE     0xFFFFFF
#define RED       0xFF0000
#define GREEN     0x00FF00
#define BLUE      0x0000FF
#define YELLOW    0xFFFF00
#define CYAN      0x00FFFF
#define MAGENTA   0xFF00FF

class RGBSpectrum : public CoefficientSpectrum<3> {
    using CoefficientSpectrum<3>::c;
public:
    // RGBSpectrum Public Methods
    //RGBSpectrum(float v = 0.f) : CoefficientSpectrum<3>(v) { }
    RGBSpectrum(SPECTRUM_RGB rgb=0)
    {
      assert(rgb >= 0 && rgb <= 0xFFFFFF);
      c[2] = static_cast<float>(((0x00FF0000 & rgb)>>16) / 255.);
      c[1] = static_cast<float>(((0x0000FF00 & rgb)>>8) / 255.);
      c[0] = static_cast<float>(((0x000000FF & rgb)) / 255.);
    }
    RGBSpectrum(const CoefficientSpectrum<3> &v)
        : CoefficientSpectrum<3>(v) { }
    RGBSpectrum(const RGBSpectrum &s, SpectrumType type = SPECTRUM_REFLECTANCE) {
        *this = s;
    }
    static RGBSpectrum FromRGB(const float rgb[3],
            SpectrumType type = SPECTRUM_REFLECTANCE) {
        RGBSpectrum s;
        s.c[0] = static_cast<float>(rgb[0] / 255.);
        s.c[1] = static_cast<float>(rgb[1] / 255.);
        s.c[2] = static_cast<float>(rgb[2] / 255.);
        assert(!s.HasNaNs());
        return s;
    }
    void ToRGB(float *rgb) const {
        rgb[0] = c[0] * 255;
        rgb[1] = c[1] * 255;
        rgb[2] = c[2] * 255;
    }
    const RGBSpectrum &ToRGBSpectrum() const {
        return *this;
    }
    void ToXYZ(float xyz[3]) const {
        RGBToXYZ(c, xyz);
    }
    static RGBSpectrum FromXYZ(const float xyz[3],
            SpectrumType type = SPECTRUM_REFLECTANCE) {
        RGBSpectrum r;
        XYZToRGB(xyz, r.c);
        return r;
    }
    float ColorFrac()
    {
      return (c[0] + c[1] + c[2]) / 3.f;
    }
    float y() const {
        const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
        return YWeight[0] * c[0] + YWeight[1] * c[1] + YWeight[2] * c[2];
    }

};


/*******************************RGBASpectrum**********************************/
class RGBASpectrum : public CoefficientSpectrum<4> 
{
    using CoefficientSpectrum<4>::c;
public:
  RGBASpectrum(SPECTRUM_RGBA rgba=0)
    {
      c[3] = static_cast<float>(((0xFF000000 & rgba)>>24) / 255.);
      c[2] = static_cast<float>(((0x00FF0000 & rgba)>>16) / 255.);
      c[1] = static_cast<float>(((0x0000FF00 & rgba)>>8) / 255.);
      c[0] = static_cast<float>(((0x000000FF & rgba)) / 255.);
    }
  RGBASpectrum(const CoefficientSpectrum<4> &v)
    : CoefficientSpectrum<4>(v) { }
   
  RGBASpectrum(const RGBASpectrum &s, SpectrumType type = SPECTRUM_REFLECTANCE)
  {
    *this = s;
  }
    
  static RGBASpectrum FromRGBA(const float rgba[4],
                               SpectrumType type = SPECTRUM_REFLECTANCE)
  {
    RGBASpectrum s;
    s.c[0] = static_cast<float>(rgba[0] / 255.);
    s.c[1] = static_cast<float>(rgba[1] / 255.);
    s.c[2] = static_cast<float>(rgba[2] / 255.);
    s.c[3] = static_cast<float>(rgba[3] / 255.);
    assert(!s.HasNaNs());
    return s;
  }

  float getAlpha() const
  {
    return c[0];
  }

  RGBSpectrum getRGB() const
  {
    RGBSpectrum rgb;
    rgb[2] = c[3];
    rgb[1] = c[2];
    rgb[0] = c[1];

    return rgb;
  }
    
  void ToRGBA(float* rgba) const 
  {
    rgba[0] = c[0] * 255;
    rgba[1] = c[1] * 255;
    rgba[2] = c[2] * 255;
    rgba[3] = c[3] * 255;
  }

  const RGBASpectrum& ToRGBASpectrum() const 
  {
    return *this;
  }

};


inline void RGBToRGBA(const RGBSpectrum& rgb, RGBASpectrum& rgba,
                      float alpha=1.f)
{
  rgba[3] = rgb[2];
  rgba[2] = rgb[1];
  rgba[1] = rgb[0];
  rgba[0] = alpha;
}


inline void RGBAToRGB(const RGBASpectrum& rgba, RGBSpectrum& rgb)
{
  rgb[2] = rgba[3];
  rgb[1] = rgba[2];
  rgb[0] = rgba[1];
}


// Spectrum Inline Functions
template <int nSamples> inline CoefficientSpectrum<nSamples>
Pow(const CoefficientSpectrum<nSamples> &s, float e) {
    CoefficientSpectrum<nSamples> ret;
    for (int i = 0; i < nSamples; ++i)
        ret.c[i] = powf(s.c[i], e);
    assert(!ret.HasNaNs());
    return ret;
}


inline Spectrum Lerp(float t, const Spectrum &s1, const Spectrum &s2) {
    return (1.f - t) * s1 + t * s2;
}



#endif //SPECTRUM
