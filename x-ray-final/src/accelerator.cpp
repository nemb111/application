#include "stdafx.h"

//#include "accelerator.h"


//_____________________________________________________________________________
Accelerator::Accelerator(void)
  : _shapesRef(NULL)
{
}


//_____________________________________________________________________________
Accelerator::Accelerator(const std::vector<Shape*>& shapes)
  : _shapesRef(&shapes)
{
}


//_____________________________________________________________________________
Accelerator::~Accelerator(void)
{
}


//_____________________________________________________________________________
void
Accelerator::addShapes(const std::vector<Shape*>& shapes)
{
  _shapesRef = &shapes;
}
