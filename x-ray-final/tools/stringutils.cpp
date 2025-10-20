#include "stdafx.h"

//#include "stringutils.h"


//_____________________________________________________________________________
std::vector<std::string> &split(std::vector<std::string> &elems,
                                const std::string &s, char delim)
{
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) {
      elems.push_back(item);
  }
  return elems;

}


//_____________________________________________________________________________
std::vector<std::string> split(const std::string &s, char delim)
{
  std::vector<std::string> elems;
  return split(elems, s, delim);
}