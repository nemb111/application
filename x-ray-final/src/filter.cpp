#include "stdafx.h"

//#include "filter.h"


//_____________________________________________________________________________
Filter::Filter(uint xWidth, uint yWidth)
 : _xWidth(xWidth), _yWidth(yWidth)
{
}


//_____________________________________________________________________________
Filter::~Filter(void)
{
}


//_____________________________________________________________________________
void
Filter::init(const Spectrum2D& spec2D)
{
}


//_____________________________________________________________________________
size_t
Filter::calcI(size_t index, int offset, size_t picSize) const
{
  if (index + offset >= 0 && index + offset < picSize)
    return index + offset;
  else if (int(index + offset) < 0)
  {
    return abs(int(index + offset) );
  }
  else
  {
    return (2*picSize - 2) - (index + offset);
  }

}


//_____________________________________________________________________________
size_t
Filter::calcPI(uint index, uint width, size_t size) const
{
  if (index + width < size)
    return index + width;

  int sign = 1;
  uint result = index;

  for (uint i=0; i<width; i++)
  {
    if (result == size - 1)
      sign *= -1;

    result += sign;
  }

  return result;
}


//_____________________________________________________________________________
size_t
Filter::calcNI(uint index, uint width) const
{
  if ((int)(index - width) >= 0)
    return index - width;

  int sign = -1;
  uint result = index;

  for (uint i=0; i<width; i++)
  {
    if (result == 0)
      sign *= -1;

    result += sign;
  }

  return result;
}


