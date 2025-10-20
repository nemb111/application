#pragma once

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <sstream>
#include <string>
#include <vector>


//std::vector<std::string> &split(std::vector<std::string> &elems,
//                                const std::stringstream& ss, char delim)
//{
//  std::stringstream tmp(ss);
//  std::string item;
//  while(std::getline(tmp, item, delim)) {
//      elems.push_back(item);
//  }
//  return elems;
//}


std::vector<std::string> &split(std::vector<std::string> &elems,
                                const std::string &s, char delim);


std::vector<std::string> split(const std::string &s, char delim);



//std::vector<std::string> split(const std::stringstream& ss, char delim)
//{
//  std::vector<std::string> elems;
//  return split(elems, ss, delim);
//}


#endif//STRINGUTILS_H_