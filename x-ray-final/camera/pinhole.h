/* Das Interface sowie Codeauszüge in dieser Datei oder der zugehörigen cpp-Datei stammen aus:
   Dem Buch "Ray Tracing from the ground up" / Seite 159ff.
*/


#pragma once

#ifndef PINHOLE_H_
#define PINHOLE_H_

#include "camera.h"


class Pinhole : public Camera
{
public:
  Pinhole();
  //Pinhole(const Film* film, ViewPlaneSampler* viewPlaneSampler);
  Pinhole(const Point& planePos, const Normal& lookDir, const Normal& upVec,
          double distToFilm);
  // Erzeugt die Kamera durch Angabe des horizontalen FOV und der tatsächlichen
  // Ausmaße der Filmebene. D.h. Anzahl der Pixel * Länge/Pixel 
  // (xres/yres * pixelSize in Klasse Film)
  Pinhole(const Point& planePos, const Normal& lookDir, const Normal& upVec,
          double viewPlaneWidth, double horizontalFOV);
  virtual ~Pinhole(void);

  //virtual bool getNextNSamples(uint nSamples, SampleList& sampleList);
  virtual bool getNextNSamples(uint nSamples, SampleList& dirIllSampleList,
                               SampleList& globIllSampleList);

  virtual void lookAt(const Point& lookAt, const Normal& upVec=Vector(0,1,0) );
  virtual void setPos(const Point& eyePos, const Normal& upVec=Vector(0,1,0) );
  virtual void setFilmDist(double distToFilm);

protected:
  //Übergib zuerst den UpVector dann den LookDir Vektor und erhalte ein
  //rechtshändisches ONB in den Vektoren _e1, _e2 und _e3
  void createONB(const Normal& upVec, const Normal& lookDir);

  Point _planePos;
  Point _eyePos;
  Normal _lookDir;
  Normal _upVec;
  double _distToFilm;

  //Einheitsvektoren des Orthonormalen Koordinatensystems
  Normal _e1;
  Normal _e2;
  Normal _e3;
};

#endif//PINHOLE_H_