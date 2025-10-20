#include "stdafx.h"

//#include "simplefilm.h"

//_____________________________________________________________________________
SimpleFilm::SimpleFilm() 
  : Film(1366, 768, 0.03, 1, 0, true)
{
}


//_____________________________________________________________________________
SimpleFilm::SimpleFilm(int xr, int yr, double pSize, float gam, 
                       float inv_gam, bool s_o_o_g)
 : Film(xr, yr, pSize, gam, inv_gam, s_o_o_g)
{

}


//_____________________________________________________________________________
SimpleFilm::~SimpleFilm()
{
}
