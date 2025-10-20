

/**********************************BIHNode************************************/

//_____________________________________________________________________________
inline BIHNode::BIHNode(int index, int axis, float clip[2])
{
  setIndex(index);
  setAxis(axis);
  Clip[0] = clip[0];
  Clip[1] = clip[1];
}


//_____________________________________________________________________________
inline BIHNode::BIHNode(int shpIndex, int nShp)
{
  setIndex(shpIndex);
  setToLeaf();
  items = nShp;
}


//_____________________________________________________________________________
inline void
BIHNode::setAxis(int axis)
{
  assert(axis >= 0 && axis <3);

  _index = (_index & 0xFFFFFFFC) + axis;
}


//_____________________________________________________________________________
inline int
BIHNode::getAxis() const
{
  return _index & 0x3;
}


//_____________________________________________________________________________
inline void
BIHNode::setIndex(int index)
{
  assert(index >= 0 && index < 0x3FFFFFFF);
  _index = (index << 2) + (_index & 3);
}


//_____________________________________________________________________________
inline int
BIHNode::getIndex() const
{
  return _index >> 2;
}


//_____________________________________________________________________________
inline void
BIHNode::setToLeaf()
{
  _index = (_index & 0xFFFFFFFC) + 3;
}


//_____________________________________________________________________________
inline bool
BIHNode::isLeaf() const
{
  return ((_index & 0x3) == 3);
}
