
#ifndef DISPLAYINFO_H_
#define DISPLAYINFO_H_

#include <Windows.h>

//_____________________________________________________________________________
// Gibt die Breite des Monitors in mm zurück
inline int 
getDispWidthMM()
{
  HWND const hwnd = 0;
  HDC const dc = ::GetDC(hwnd);

  return ::GetDeviceCaps(dc, HORZSIZE);
}


//_____________________________________________________________________________
// Gibt die Breite des Monitors in cm zurück
inline float
getDispWidthCM()
{
  return getDispWidthMM() / 10.f;
}


//_____________________________________________________________________________
// Gibt die Höhe des Monitors in mm zurück
inline int 
getDispHeightMM()
{
  HWND const hwnd = 0;
  HDC const dc = ::GetDC(hwnd);

  return ::GetDeviceCaps(dc, VERTSIZE);
}


//_____________________________________________________________________________
// Gibt die Höhe des Monitors in cm zurück
inline float
getDispHeightCM()
{
  return getDispHeightMM() / 10.f;
}


//_____________________________________________________________________________
// Gibt die Anzahl der Pixel in der Breite zurück
inline int
getDispWidthRes()
{
  HWND const hwnd = 0;
  HDC const dc = ::GetDC(hwnd);

  return ::GetDeviceCaps(dc, VERTRES);
}


//_____________________________________________________________________________
// Gibt die Anzahl der Pixel in der Höhe zurück
inline int
getDispHeightRes()
{
  HWND const hwnd = 0;
  HDC const dc = ::GetDC(hwnd);

  return ::GetDeviceCaps(dc, HORZRES);
}


//_____________________________________________________________________________
// Gibt die Pixeldichte des Monitors in Pixel pro mm² zurück
inline float
getDispPxlDensMM()
{
  return (getDispHeightRes() * getDispWidthRes() ) /
          static_cast<float>(getDispHeightMM() * getDispWidthMM() );
}


//_____________________________________________________________________________
// Gibt die Pixeldichte des Monitors in Pixel pro cm² zurück
inline float
getDispPxlDensCM()
{
  return (getDispHeightRes() * getDispWidthRes() ) /
          static_cast<float>(getDispHeightCM() * getDispWidthCM() );
}




//_____________________________________________________________________________
// Gibt die Größe (Fläche) eines Pixels in mm²/(1 pxl) zurück
inline float
getDispPxlSizeMM()
{
  return 1.f/getDispPxlDensMM();
}


//_____________________________________________________________________________
// Gibt die Größe (Fläche) eines Pixels in cm²/(1 pxl) zurück
inline float
getDispPxlSizeCM()
{
  return 1.f/getDispPxlDensCM();
}



#endif //DISPLAYINFO_H_