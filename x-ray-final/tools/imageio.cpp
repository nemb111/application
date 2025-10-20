#include "stdafx.h"

//#include "imageio.h"

namespace IMAGEIO
{

//_____________________________________________________________________________
void
writeImage(const std::wstring& filename,unsigned int xres,
          unsigned int yres, const PixelDataRGB& pixelDataRGB)
{
  float rgb[3];
  wchar_t dir[256];
  ILubyte ilRGB[3];
  ILuint* imageIds = new ILuint;

  ilInit(); /* Initialization of DevIL */
  /* generate DevIL Image IDs */
	ilGenImages(1, imageIds); /* Generation of numTextures image names */
  ilBindImage(*imageIds); /* Binding of DevIL image name */
  ILboolean success = ilTexImage(xres, yres, 1, 3, IL_RGB, 
                                 IL_UNSIGNED_BYTE, NULL);
  if (success == false)
  {
    EOUT("DevIl konnte die zugrunde liegenden Bilddaten nicht verändern.");
    std::cin.get();
    exit(1);
  }
  for (size_t y=0; y<yres; y++)
  {
    for (size_t x=0; x<xres; x++)
    {
      pixelDataRGB[y][x].ToRGB(rgb);
      ilRGB[2] = static_cast<unsigned char>(rgb[0]);
      ilRGB[1] = static_cast<unsigned char>(rgb[1]);
      ilRGB[0] = static_cast<unsigned char>(rgb[2]);
      ilSetPixels((ILint)x, (ILint)y, 0, 1, 1, 1, IL_RGB, IL_UNSIGNED_BYTE,
                  &ilRGB );

    }
  }
  // Erzeuge den Zielordner falls nicht vorhanden
  _wsplitpath_s(filename.c_str(), NULL, 0, dir, 256, NULL, 0, NULL, 0);
  if (!(CreateDirectory(dir, NULL) || ERROR_ALREADY_EXISTS == GetLastError() ) )
  {
    WOUT(_T("Das Verzeichnis \"") << dir <<
         _T("\" konnte nicht erzeugt werden") );
  }
  
  ilEnable(IL_FILE_OVERWRITE);
  if (!ilSaveImage(filename.c_str() ) )
  {
    WOUT(_T("DevIL konnte das Bild nicht speichern.") );
  };
  ilDisable(IL_FILE_OVERWRITE);
  ilDeleteImages(1, imageIds); /* Because we have already copied
    image data into texture data we can release memory used by image. */
  delete imageIds;
}


//_____________________________________________________________________________
void
writeImage(const std::wstring& filename,unsigned int xres,
               unsigned int yres, const PixelDataRGBA& pixelDataRGBA)
{
  float rgba[4];
  wchar_t dir[256];
  ILubyte ilRGBA[4];
  ILuint* imageIds = new ILuint;

  ilInit(); /* Initialization of DevIL */
  /* generate DevIL Image IDs */
	ilGenImages(1, imageIds); /* Generation of numTextures image names */
  ilBindImage(*imageIds); /* Binding of DevIL image name */
  ILboolean success = ilTexImage(xres, yres, 1, 4, IL_RGBA, 
                                 IL_UNSIGNED_BYTE, NULL);
  if (success == false)
  {
    EOUT("DevIl konnte die zugrunde liegenden Bilddaten nicht verändern.");
    std::cin.get();
    exit(1);
  }
  for (size_t y=0; y<yres; y++)
  {
    for (size_t x=0; x<xres; x++)
    {
      pixelDataRGBA[y][x].ToRGBA(rgba);
      ilRGBA[3] = static_cast<unsigned char>(rgba[0]);
      ilRGBA[2] = static_cast<unsigned char>(rgba[1]);
      ilRGBA[1] = static_cast<unsigned char>(rgba[2]);
      ilRGBA[0] = static_cast<unsigned char>(rgba[3]);
      ilSetPixels((ILint)x, (ILint)y, 0, 1, 1, 1, IL_RGBA, IL_UNSIGNED_BYTE,
                  &ilRGBA );

    }
  }
  // Erzeuge den Zielordner falls nicht vorhanden
  _wsplitpath_s(filename.c_str(), NULL, 0, dir, 256, NULL, 0, NULL, 0);
  if (!(CreateDirectory(dir, NULL) || ERROR_ALREADY_EXISTS == GetLastError() ) )
  {
    WOUT(_T("Das Verzeichnis \"") << dir <<
         _T("\" konnte nicht erzeugt werden") );
  }

  ilEnable(IL_FILE_OVERWRITE);
  if (!ilSaveImage(filename.c_str() ) )
  {
    WOUT(_T("DevIL konnte das Bild nicht speichern.") );
  };
  ilDisable(IL_FILE_OVERWRITE);
  ilDeleteImages(1, imageIds); /* Because we have already copied
    image data into texture data we can release memory used by image. */
  delete imageIds;

}


//_____________________________________________________________________________
void
readImage(const std::wstring& filename, PixelDataRGB& pixelDataRGB)
{
  ILubyte rgb[3];
  ILint width, height;
  ILuint* imageIds = new ILuint;

  pixelDataRGB.clear();

  ilInit(); /* Initialization of DevIL */
  /* generate DevIL Image IDs */
	ilGenImages(1, imageIds); /* Generation of numTextures image names */
  ilBindImage(*imageIds); /* Binding of DevIL image name */

  ILboolean success = ilLoadImage(filename.c_str() );

  if (!success)
  {
    EOUT("DevIl konnte das Bild mit dem Pfad \"" << filename.c_str()
          << "\" nicht laden");
    std::cin.get();
    exit(1);
  }

  width = ilGetInteger(IL_IMAGE_WIDTH);
  height = ilGetInteger(IL_IMAGE_HEIGHT);
  pixelDataRGB.reserve(height);
  for (int j=0; j<height; j++)
  {
    std::vector<RGBSpectrum> pxlRow;
    pxlRow.reserve(width);

    for (int k=0; k<width; k++)
    {
      ilCopyPixels(k, j, 0, 1, 1, 1, IL_RGB, IL_UNSIGNED_BYTE, &rgb);
      
      pxlRow.push_back(RGBSpectrum((rgb[0]<<16) + (rgb[1]<<8) +
                                   (rgb[2]) ) );
    }
    pixelDataRGB.push_back(pxlRow);
  }

  ilDeleteImages(1, imageIds); /* Because we have already copied
  image data into texture data we can release memory used by image. */
  delete imageIds;

}


//_____________________________________________________________________________
void
readImage(const std::wstring& filename, PixelDataRGBA& pixelDataRGBA)
{
  //ILubyte rgba[4];
  ILint width, height;
  ILuint* imageIds = new ILuint;


  ilInit(); /* Initialization of DevIL */
  /* generate DevIL Image IDs */
	ilGenImages(1, imageIds); /* Generation of numTextures image names */
  ilBindImage(*imageIds); /* Binding of DevIL image name */

  ILboolean success = ilLoadImage(filename.c_str() );

  if (!success)
  {
    EOUT("DevIl konnte das Bild mit dem Pfad \"" << filename.c_str()
          << "\" nicht laden");
    std::cin.get();
    exit(1);
  }

  success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );

  if (!success)
  {
    EOUT("DevIl konnte das Bild mit dem Pfad \"" << filename.c_str()
          << "\" nicht in RGBA konvertieren");
    std::cin.get();
    exit(1);
  }

  width = ilGetInteger(IL_IMAGE_WIDTH);
  height = ilGetInteger(IL_IMAGE_HEIGHT);

  pixelDataRGBA.clear();
  pixelDataRGBA.reserve(height);

  
  std::vector<RGBASpectrum> pxlRow;
  pxlRow.reserve(width);
  //for (int j=0; j<height; j++)
  //{   
  //  pxlRow.clear();
  //  for (int k=0; k<width; k++)
  //  {
  //    ilCopyPixels(k, j, 0, 1, 1, 1, IL_RGBA, IL_UNSIGNED_BYTE, &rgba);
  //    
  //    pxlRow.push_back(RGBASpectrum((rgba[0]<<24) + (rgba[1]<<16) +
  //                                  (rgba[2]<<8) + (rgba[3]) ) );
  //  }
  //  pixelDataRGBA.push_back(pxlRow);
  //}

  ILubyte* rgba = new ILubyte[4*width];
  for (int j=0; j<height; j++)
  {   
    pxlRow.clear();
    ilCopyPixels(0, j, 0, width, 1, 1, IL_RGBA, IL_UNSIGNED_BYTE, rgba);
      
    for (int y=0; y<width*4; y+=4)
    {
      pxlRow.push_back(RGBASpectrum((rgba[y]<<24) + (rgba[y+1]<<16) +
                                    (rgba[y+2]<<8) + (rgba[y+3]) ) );
    }  
    
    pixelDataRGBA.push_back(pxlRow);
  }

  


  ilDeleteImages(1, imageIds); /* Because we have already copied
  image data into texture data we can release memory used by image. */
  delete imageIds;
  delete rgba;
}



}