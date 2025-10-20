#include "stdafx.h"

//#include <fstream>
//#include <sstream>
//
//#include "options.h"
//#include "ppm.h"


//_____________________________________________________________________________
void PPM::writeFile(unsigned int xres, unsigned int yres,
                    unsigned int colordepth, const Spectrum2D& spectrumArray)
{
  //Schreibe eine einfache ppm Datei
  std::wofstream outfile(Options::getInstance().outfile + _T(".ppm"), 
                         std::ios::binary);
  if (!outfile.is_open() )
  {
    EOUT("Ausgabedatei konnte nicht erstellt werden");
    std::cin.get();
    exit(1);
  }


  outfile << "P6" << "\n"
          << "#" << Options::getInstance().outfile << ".ppm\n"
          << xres << " " << yres << "\n"
          << colordepth << "\n";

  float rgb[3];
  for (int y=yres-1; y>=0; y--)
  {
    for (uint x=0; x<xres; x++)
    {
      spectrumArray[y][x].ToRGB(rgb);
      outfile << char(rgb[2]) << char(rgb[1]) << char(rgb[0]);
    }
  }
}


//_____________________________________________________________________________
void PPM::writeFile(const std::wstring& filename, unsigned int xres,
                    unsigned int yres, unsigned int colordepth,
                    const Spectrum2D& spectrumArray)
{
  //Schreibe eine einfache ppm Datei
  std::wofstream outfile(filename+ L".ppm", std::ios::binary);
  if (!outfile.is_open() )
  {
    EOUT("Ausgabedatei konnte nicht erstellt werden");
    exit(1);
  }


  outfile << L"P6" << L"\n"
          << L"#" << filename << L".ppm\n"
          << xres << L" " << yres << L"\n"
          << colordepth << L"\n";

  float rgb[3];
  for (int y=yres-1; y>=0; y--)
  {
    for (uint x=0; x<xres; x++)
    {
      spectrumArray[y][x].ToRGB(rgb);
      outfile << char(rgb[2]) << char(rgb[1]) << char(rgb[0]);
    }
  }
}


//_____________________________________________________________________________
void PPM::readFile(unsigned int& xres, unsigned int& yres, unsigned int& colordepth,
                   Spectrum2D& spectrumArray)
{
  std::ifstream infile(Options::getInstance().outfile+_T(".ppm"), std::ios::binary);
  if (!infile.is_open() )
  {
    EOUT("Eingabedatei konnte nicht geöffnet werden");
    std::cin.get();
    exit(1);
  }

  infile.ignore(256, '\n');
  infile.ignore(256, '\n');
  infile >> xres;
  infile >> yres;
  infile >> colordepth;
  infile.ignore(256, '\n');

  spectrumArray.resize(yres);

  char rgb[3];
  float f_rgb[3];
  for (int y=yres-1; y>=0; y--)
  {
    for (uint x=0; x<xres; x++)
    {
      infile.get(rgb[2]);
      infile.get(rgb[1]);
      infile.get(rgb[0]);
      for (int i=0; i<3; i++)
        f_rgb[i] = (unsigned char) rgb[i];

      spectrumArray[y].push_back(Spectrum().FromRGB(f_rgb) );
    }
  }

}
