#include "stdafx.h"

//#include "renderer.h"


//_____________________________________________________________________________
void
Renderer::init(const World* world)
{
  _world = world;
}


//_____________________________________________________________________________
bool
Renderer::Render(const SampleList& sampleList, uint iteration) const
{
  Spectrum resSpec(BLACK);

  for (size_t i=0; i<sampleList.size(); i++)
  {
    // Bedingung die Überprüft, ob nur ein einziger Pixel gerendert werden soll
    if (_debugPixelSet && !(sampleList[i].pxlIdxX == _debugPixelX &&
        sampleList[i].pxlIdxY == _debugPixelY) )
      continue;

    resSpec = BLACK;
    for (size_t j=0; j<sampleList[i].rays.size(); j++)
    {
      resSpec += Li(sampleList[i].rays[j], 0, 1.0f) * sampleList[i].weigths[j];

    }
    
    writeBackSample(sampleList[i].pxlIdxX, sampleList[i].pxlIdxY, resSpec);
  }

  return true;
}


//_____________________________________________________________________________
//void
//Renderer::GlobIllRender(World* world, PrioSampleList& prioSampleList)
//{
//  _world = world;
//  float priority;
//  Spectrum resSpec(BLACK);
//
//
//  for (size_t i=0; i<prioSampleList.size(); i++)
//  {
//    // Bedingung die Überprüft, ob nur ein einziger Pixel gerendert werden soll
//    if (_debugPixelSet && !(prioSampleList[i].second.pxlIdxX == _debugPixelX &&
//        prioSampleList[i].second.pxlIdxY == _debugPixelY) )
//    {
//      prioSampleList[i].first = -std::numeric_limits<float>::max();
//      continue;
//    }
//
//    resSpec = BLACK;
//    for (size_t j=0; j<prioSampleList[i].second.rays.size(); j++)
//    {
//      resSpec += globalLi(prioSampleList[i].second.rays[j], 0, 1.0f) * 
//                 prioSampleList[i].second.weigths[j];
//
//    }
//    // Setze neue Priorität
//    priority = resSpec.magnitude() - 
//               _world->film->getGlobIllPixelSpectrum(
//                 prioSampleList[i].second.pxlIdxX, 
//                 prioSampleList[i].second.pxlIdxY).magnitude();
//    prioSampleList[i].first += priority;
//
//
//    if (priority > 0)
//    {
//      writeBackGlobIllSample(prioSampleList[i].second.pxlIdxX, 
//                             prioSampleList[i].second.pxlIdxY, resSpec);
//    }
//    else
//      prioSampleList[i].first -= 0.1f;
//  }
//}


//_____________________________________________________________________________
void
Renderer::writeBackSample(uint pxlIdxX, uint pxlIdxY, const Spectrum& spectrum)
const
{
  _world->film->setDirIllPixelSpectrum(pxlIdxX, pxlIdxY, spectrum);
  _world->guiComm->updateBitmap
    (pxlIdxX, pxlIdxY, _world->film->getResPixelSpectrum(pxlIdxX, pxlIdxY) );
}


//_____________________________________________________________________________
void 
Renderer::writeBackGlobIllSample(uint pxlIdxX, uint pxlIdxY,
                                 const Spectrum& spectrum) const
{
  _world->film->setGlobIllPixelSpectrum(pxlIdxX, pxlIdxY, spectrum);
  _world->guiComm->updateBitmap
    (pxlIdxX, pxlIdxY, _world->film->getResPixelSpectrum(pxlIdxX, pxlIdxY) );
}

//_____________________________________________________________________________
void
Renderer::setDebugPixel(uint debugPixelX, uint debugPixelY)
{
  _debugPixelSet = true;
  _debugPixelX = debugPixelX;
  _debugPixelY = debugPixelY;


  IOUT("Rendere einzelnen Pixel mit den Koordinaten x: " << _debugPixelX <<
       ", y: " << _debugPixelY);
}