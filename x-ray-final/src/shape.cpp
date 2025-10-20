#include "stdafx.h"

//#include "debugmaterial.h"
//#include "shape.h"

//_____________________________________________________________________________
Shape::Shape(const stdstring& name/*=_T("")*/, bool throwsShadow/*=true*/)
  : _name(name), _material(NULL), _matset(false), _throwsShadow(throwsShadow),
    _bv(NULL)
{
}


//_____________________________________________________________________________
Shape::~Shape(void)
{
}


//_____________________________________________________________________________
void
Shape::setMaterial(const Material* material)
{
  _matset = true;
  _material = material;
}


//_____________________________________________________________________________
void
Shape::setThrowsShadow(bool throwsShadow)
{
  _throwsShadow = throwsShadow;
}


//_____________________________________________________________________________
const BV&
Shape::getBV() const
{
  assert(_bv);
  return *_bv;
}