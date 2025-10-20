
#include "stdafx.h"

#include "stopwatch.h"


//#define SPLIT_EVEN


// Folgende Structs dienen dazu den Overhead beim Aufruf von Methoden zu
// reduzieren
struct GetSplitAxisParam
{
  const std::vector<std::pair<size_t, float> >& xSortShp;
  const std::vector<std::pair<size_t, float> >& ySortShp;
  const std::vector<std::pair<size_t, float> >& zSortShp;
  const std::array<size_t, 2>& xPartitionIds;
  const std::array<size_t, 2>& yPartitionIds;
  const std::array<size_t, 2>& zPartitionIds;
  const AABB& pAABB;
};


struct ClipAndPartitionParam
{
  const std::vector<std::pair<size_t, float> >& xSortShp;
  const std::vector<std::pair<size_t, float> >& ySortShp;
  const std::vector<std::pair<size_t, float> >& zSortShp;
  const std::array<size_t, 2>& xPartitionIds;
  const std::array<size_t, 2>& yPartitionIds;
  const std::array<size_t, 2>& zPartitionIds;
  std::list<std::pair<size_t, Point> >& shpIds;
  float (&clip)[2];
  int axis;
  float axisValue;
};


struct StackData
{
  AABB curAABB;
  std::array<size_t, 2> xPartitionIds;
  std::array<size_t, 2> yPartitionIds;
  std::array<size_t, 2> zPartitionIds;
  std::list<std::pair<size_t, Point> > shpIds;
  size_t treeIdx;
  int depth;
  bool isLeftChild;    
};


struct TraversalStack
{
  const BIHNode* node;
  float tmin;
  float tmax;
};


//Die Funktion sotiert aufsteigend nach dem Achsen-Mittelpunkt
bool sortMidpoint(const std::pair<size_t, float>& pair1,
                  const std::pair<size_t, float>& pair2)
{
  return pair1.second < pair2.second;
}


void
BIH::clipAndPartition(const ClipAndPartitionParam& capp,
                      std::array<size_t, 2>& lPartitionIds,
                      std::array<size_t, 2>& rPartitionIds,
                      std::list<std::pair<size_t, Point> >& lShpIds,
                      std::list<std::pair<size_t, Point> >& rShpIds)
{
  const std::vector<std::pair<size_t, float> >& xSortShp = capp.xSortShp;
  const std::vector<std::pair<size_t, float> >& ySortShp = capp.ySortShp;
  const std::vector<std::pair<size_t, float> >& zSortShp = capp.zSortShp;
  const std::array<size_t, 2>& xPartitionIds = capp.xPartitionIds;
  const std::array<size_t, 2>& yPartitionIds = capp.yPartitionIds;
  const std::array<size_t, 2>& zPartitionIds = capp.zPartitionIds;
  std::list<std::pair<size_t, Point> > & shpIds = capp.shpIds;
  float (&clip)[2] = capp.clip;
  int axis = capp.axis;
  float axisValue = capp.axisValue;
  std::vector<std::pair<size_t, float> >::const_iterator 
    begin, end;


#ifdef SPLIT_EVEN
  shpIds.sort(
    [&axis](const std::pair<size_t, Point>& el1, 
            const std::pair<size_t, Point>& el2) -> bool
            {return el1.second[axis] < el2.second[axis];});
  size_t offset = (shpIds.size() / 2) + 1;

  auto lItEnd = shpIds.begin();
  std::advance(lItEnd, offset);

  auto low=std::lower_bound(
    shpIds.begin(), lItEnd, lItEnd->second[axis],
    [&axis](const std::pair<size_t, Point>& el1, 
      const float& el2) -> bool
        {return el1.second[axis] < el2; });


  lShpIds.splice(lShpIds.begin(), shpIds, shpIds.begin(), low);
  rShpIds.splice(rShpIds.begin(), shpIds);

  for (auto it=lShpIds.cbegin(); it!=lShpIds.cend(); it++)
    clip[0] = std::max(clip[0], dynamic_cast<const AABB&>(
        (*_shapesRef)[it->first ]->getBV() ).maxSlab[axis] );


  for (auto it=rShpIds.cbegin(); it!=rShpIds.cend(); it++)
    clip[1] = std::min(clip[1], dynamic_cast<const AABB&>(
        (*_shapesRef)[it->first ]->getBV() ).minSlab[axis] );



#else
  lShpIds.clear();
  rShpIds.clear();


  // Zuerst werden die Objekte Partitioniert
  switch (axis)
  {
  case 0:
    {
      begin = xSortShp.begin() + xPartitionIds[0];
      end   = xSortShp.begin() + xPartitionIds[1];
      lPartitionIds = rPartitionIds = xPartitionIds;
      break;
    }
  case 1:
    {
      begin = ySortShp.begin() + yPartitionIds[0];
      end   = ySortShp.begin() + yPartitionIds[1];
      lPartitionIds = rPartitionIds = yPartitionIds;
      break;
    }

  case 2:
    {
      begin = zSortShp.begin() + zPartitionIds[0];
      end   = zSortShp.begin() + zPartitionIds[1];
      lPartitionIds = rPartitionIds = zPartitionIds;
      break;
    }

  }

  auto low=std::lower_bound(begin, end, axisValue,
                            [](const std::pair<const size_t, float>& el1,
                            const float& el2)
                            {return el1.second < el2;} );

  lPartitionIds[1] += low - end;
  rPartitionIds[0] += low - begin;

  
  switch (axis)
  {
  case 0:
    begin = xSortShp.begin() + lPartitionIds[0];
    end   = xSortShp.begin() + lPartitionIds[1];


    break;
  case 1:
    begin = ySortShp.begin() + lPartitionIds[0];
    end   = ySortShp.begin() + lPartitionIds[1];


    break;
  case 2:
    begin = zSortShp.begin() + lPartitionIds[0];
    end   = zSortShp.begin() + lPartitionIds[1];


    break;
  }


  // Überprüfe ob die Shape Id in der linken Partition vorkommt, falls nicht
  // muss Sie in der rechten sein
  for (auto it=shpIds.begin(); it!=shpIds.end(); it++)
  {
    auto low=std::lower_bound(begin, end, it->second[axis],
      [](const std::pair<size_t, float>& el1, 
      const float& el2) -> bool
        {return el1.second < el2; });
    auto upper=std::upper_bound(begin, end, it->second[axis],
      [](const float& el1, 
      const std::pair<size_t, float>& el2) -> bool
        {return el1 < el2.second; });
    
    /*for (auto it2=low; it2!=upper; it2++)
    {
      if (it2->first == it->first)
      {
        clip[0] = std::max(clip[0], dynamic_cast<const AABB&>(
          (*_shapesRef)[it->first ]->getBV() ).maxSlab[axis] );
        
        auto tmp=it;
        it--;
        lShpIds.splice(lShpIds.end(), shpIds, tmp);

        break;
      }

    }*/
    if (upper - low > 0)
    {
      clip[0] = std::max(clip[0], dynamic_cast<const AABB&>(
        (*_shapesRef)[it->first ]->getBV() ).maxSlab[axis] );
        
      auto tmp=it;
      it--;
      lShpIds.splice(lShpIds.end(), shpIds, tmp);
    }

  }
  // Der Rest, der jetzt noch in shpIds drin ist muss in rShpIds verschoben werden
  rShpIds.splice(rShpIds.begin(), shpIds);

  for (auto it=rShpIds.cbegin(); it!=rShpIds.cend(); it++)
    clip[1] = std::min(clip[1], dynamic_cast<const AABB&>(
        (*_shapesRef)[it->first ]->getBV() ).minSlab[axis] );
#endif //SPLIT_EVEN


}


/************************************BIH**************************************/
//_____________________________________________________________________________
BIH::BIH(void)
  : _globalAABB(AABB::empty() ) 
{
}


//_____________________________________________________________________________
BIH::BIH(const std::vector<Shape*>& shapes)
  : Accelerator(shapes), _globalAABB(AABB::empty() )
{
}


//_____________________________________________________________________________
BIH::~BIH(void)
{
}


//_____________________________________________________________________________
bool
BIH::create()
{ 
  //IOUT("Erzeuge die BIH 20x um Performance zu messen.");
  Stopwatch::start(_T("bih") );
  //for (int i=0; i<20; i++)
    create(64, 2);
  IOUT("Erzeugung der BIH dauerte: " << Stopwatch::end(_T("bih") )<<"ms" );

  //std::cin.get();
  //exit(EXIT_SUCCESS);
  return true;
}


//_____________________________________________________________________________
bool
BIH::create(int maxDepth, int nShpPerLeaf)
{
  StackData currNode = {_globalAABB,
                       {0, _shapesRef->size() },
                       {0, _shapesRef->size() },
                       {0, _shapesRef->size() },
                       std::list<std::pair<size_t, Point> >(),
                       0, 1, true};


  int axis;
  float axisValue;
  size_t treeIdx = 0;
  float clip[2] = {-std::numeric_limits<float>::max(), 
                   std::numeric_limits<float>::max()};
  AABB laabb, raabb;
  std::array<size_t, 2> lPartitionIds = {0}, rPartitionIds = {0};
  std::list<std::pair<size_t, Point> > lShpIds, rShpIds;


  std::vector<std::pair<size_t, float> > xSortShp;
  std::vector<std::pair<size_t, float> > ySortShp;
  std::vector<std::pair<size_t, float> > zSortShp;

 
  std::stack<StackData> stack;
 

  // Member zurücksetzen
  _globalAABB = AABB();
  _root       = BIHNode();
  _bihTree.clear();
  _potShp.clear();


  // Alle Container an die benötigte größe anpassen
  xSortShp.reserve(_shapesRef->size() );
  ySortShp.reserve(_shapesRef->size() );
  zSortShp.reserve(_shapesRef->size() );

  size_t nLeaves = _shapesRef->size() / nShpPerLeaf + 1;
  size_t bihTreeSize = 
    static_cast<size_t>(
      std::min((std::pow(2., maxDepth) - 2),
                static_cast<double>(nLeaves + (nLeaves - 1) ) ) );

  _potShp.reserve(nLeaves);
  _bihTree.reserve(bihTreeSize);


  //Erzeugen der globalen AABB und der Shape-index/Mittelpunkt Map
  for (size_t i=0; i<_shapesRef->size(); i++)
  {
    const AABB& aabb = dynamic_cast<const AABB&>((*_shapesRef)[i]->getBV() );
    Point p = aabb.getCentroid();
    _globalAABB.extend(aabb );

    xSortShp.push_back(std::make_pair(i, static_cast<float>(p.x) ) );
    ySortShp.push_back(std::make_pair(i, static_cast<float>(p.y) ) );
    zSortShp.push_back(std::make_pair(i, static_cast<float>(p.z) ) );
    currNode.shpIds.push_back(std::make_pair(i, p) );
  }
  

  //Sortieren der Shapes anhand der entsprechenden Achsen
  std::sort(xSortShp.begin(), xSortShp.end(), sortMidpoint);
  std::sort(ySortShp.begin(), ySortShp.end(), sortMidpoint);
  std::sort(zSortShp.begin(), zSortShp.end(), sortMidpoint);




  // Erzeugen den Wurzelknotens
  // Teste zu Beginn die Blattbedingung
  if ((currNode.shpIds.size() <= nShpPerLeaf) || 
      (currNode.depth >= maxDepth) )
  {
    if (currNode.shpIds.size() == 0)
    {
      _root = BIHNode(0, 0);
      return false;
    }
    else
    {
      std::vector<size_t> tmp;
      tmp.reserve(currNode.shpIds.size() );
      for (auto it=currNode.shpIds.begin(); it!=currNode.shpIds.end(); it++)
      {
        tmp.push_back(it->first);
      }
      _potShp.push_back(tmp);
      _root = BIHNode(static_cast<int>(_potShp.size() - 1), 
                          static_cast<int>(tmp.size() ) );
    }

    return true;
  }
  // Es handelt sich um einen inneren Knoten. Partitioniere die Shapes
  // und teste ob es sich bei den Kindern um innere Knoten oder Blätter
  // handelt.
  /*GetSplitAxisParam gsap = 
      {xSortShp, ySortShp, zSortShp, currNode.xPartitionIds,
       currNode.yPartitionIds, currNode.zPartitionIds,
       _globalAABB};*/

  getSplitAxis(/*gsap,*/_globalAABB, laabb, raabb, axis, axisValue);


  clip[0] = -std::numeric_limits<float>::max();
  clip[1] = std::numeric_limits<float>::max();

  ClipAndPartitionParam capp = 
    {xSortShp, ySortShp, zSortShp, currNode.xPartitionIds, 
     currNode.yPartitionIds, currNode.zPartitionIds, 
     currNode.shpIds, clip, axis, axisValue};

  clipAndPartition(capp, lPartitionIds, rPartitionIds, 
                   lShpIds, rShpIds);

  _root = BIHNode(0, axis, clip);


  // Zuerst den rechten Kindknoten in den Stack..
  currNode.curAABB = std::move(raabb);
  currNode.depth   = 2;
  currNode.treeIdx = 0;
  currNode.isLeftChild = false;
  currNode.shpIds = std::move(rShpIds);
  switch (axis)
  {
  case 0:
    currNode.xPartitionIds = rPartitionIds;
    break;
  case 1:
    currNode.yPartitionIds = rPartitionIds;
    break;
  case 2:
    currNode.zPartitionIds = rPartitionIds;
    break;
  }
  stack.push(currNode);
  // .. dann den linken
  currNode.curAABB = std::move(laabb);
  currNode.depth   = 2;
  currNode.treeIdx = 0;
  currNode.isLeftChild = true;
  currNode.shpIds = std::move(lShpIds);
  switch (axis)
  {
  case 0:
    currNode.xPartitionIds = lPartitionIds;
    break;
  case 1:
    currNode.yPartitionIds = lPartitionIds;
    break;
  case 2:
    currNode.zPartitionIds = lPartitionIds;
    break;
  }
  stack.push(std::move(currNode) );




  // Jetzt den restlichen Baum erzeugen
  while(stack.size() > 0)
  {
     currNode = std::move(stack.top() );
     stack.pop();

     // Füge dem Baum ein Array hinzu in dem die Knoten gespeichert werden
     // können.
     if (_bihTree.size() <= currNode.treeIdx)
      _bihTree.push_back(std::array<BIHNode, 2>() );



    // Teste zu Beginn die Blattbedingung
    if ((currNode.shpIds.size() <= nShpPerLeaf) ||
        (currNode.depth >= maxDepth) )
    {
      if (currNode.shpIds.size() == 0)
      {
        if (currNode.isLeftChild)
          _bihTree[currNode.treeIdx][0] = BIHNode(0, 0);
        else
          _bihTree[currNode.treeIdx][1] = BIHNode(0, 0);

      }
      else
      {
        std::vector<size_t> tmp;
        tmp.reserve(currNode.shpIds.size() );
        for (auto it=currNode.shpIds.begin(); it!=currNode.shpIds.end(); it++)
        {
          tmp.push_back(it->first);
        }
        _potShp.push_back(tmp);
        if (currNode.isLeftChild)
          _bihTree[currNode.treeIdx][0] = 
            BIHNode(static_cast<int>(_potShp.size() - 1),
                        static_cast<int>(tmp.size() ) );
        else
          _bihTree[currNode.treeIdx][1] = 
            BIHNode(static_cast<int>(_potShp.size() - 1),
                        static_cast<int>(tmp.size() ) );
      }

      continue;
    }
    // Es handelt sich um einen inneren Knoten. Partitioniere die Shapes
    // und teste ob es sich bei den Kindern um innere Knoten oder Blätter
    // handelt.
    /*GetSplitAxisParam gsap = 
      {xSortShp, ySortShp, zSortShp, currNode.xPartitionIds,
       currNode.yPartitionIds, currNode.zPartitionIds,
       currNode.curAABB};*/

    getSplitAxis(/*gsap,*/currNode.curAABB, laabb, raabb, axis, axisValue);


    clip[0] = -std::numeric_limits<float>::max();
    clip[1] = std::numeric_limits<float>::max();

    ClipAndPartitionParam capp = 
      {xSortShp, ySortShp, zSortShp, currNode.xPartitionIds, 
       currNode.yPartitionIds, currNode.zPartitionIds,
       currNode.shpIds, clip, axis, axisValue};

    clipAndPartition(capp, lPartitionIds, rPartitionIds,
                     lShpIds, rShpIds);
    
    treeIdx++;
    
    if (currNode.isLeftChild)
      _bihTree[currNode.treeIdx][0] =
        BIHNode(static_cast<int>(treeIdx), axis, clip);
    else
      _bihTree[currNode.treeIdx][1] =
        BIHNode(static_cast<int>(treeIdx), axis, clip);

    // Zuerst den rechten Kindknoten in den Stack..
    currNode.curAABB = std::move(raabb);
    currNode.depth   = currNode.depth + 1;
    currNode.treeIdx = treeIdx;
    currNode.isLeftChild = false;
    currNode.shpIds = std::move(rShpIds);
    switch (axis)
    {
    case 0:
      currNode.xPartitionIds = rPartitionIds;
      break;
    case 1:
      currNode.yPartitionIds = rPartitionIds;
      break;
    case 2:
      currNode.zPartitionIds = rPartitionIds;
      break;
    }

    stack.push(currNode);
    // .. dann den linken
    currNode.curAABB = std::move(laabb);
    currNode.isLeftChild = true;
    currNode.shpIds = std::move(lShpIds);
    switch (axis)
    {
    case 0:
      currNode.xPartitionIds = lPartitionIds;
      break;
    case 1:
      currNode.yPartitionIds = lPartitionIds;
      break;
    case 2:
      currNode.zPartitionIds = lPartitionIds;
      break;
    }

    stack.push(std::move(currNode) );
  }


  _bihTree.shrink_to_fit();
  _potShp.shrink_to_fit();
  return true;
}


//_____________________________________________________________________________
Accelerator*
BIH::clone() const
{
  Accelerator* accel = new BIH(*this);
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
Accelerator*
BIH::newInstance() const
{
  Accelerator* accel = new BIH;
  GBin.mrkDel(accel);

  return accel;
}


//_____________________________________________________________________________
bool
BIH::hit(const Ray& ray, size_t& shpIdx, double* t/*=NULL*/) const
{
  return hitCalc(ray, shpIdx, t);
}


//_____________________________________________________________________________
bool
BIH::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
  size_t shpIdx;

  if (hitCalc(ray, shpIdx, &tmin))
    return (*_shapesRef)[shpIdx]->hit(ray, tmin, sr);
  else
    return false;

}


//_____________________________________________________________________________
bool
BIH::shadowHit(const Ray& ray, double& tmin) const
{
  size_t shpIdx;

  if (hitCalc(ray, shpIdx, &tmin))
    return (*_shapesRef)[shpIdx]->getThrowsShadow();
  else
    return false;
}


//_____________________________________________________________________________
bool
BIH::hitCalc(const Ray& ray, size_t& shpIdx, double* t/*=NULL*/) const
{
  float tmin, tmax;

  if (_globalAABB.hit(ray, tmin, tmax) == false)
    return false;


  double tTmp = std::numeric_limits<double>::max();
  size_t shpIdxTmp = 0;
  bool   isCloser = false;
  

  // Überprüfen ob die Wurzel ein Blatt ist
  if (_root.isLeaf() )
  {
    size_t potShpIdx = _root.getIndex();
    size_t potShapesSize = _potShp[potShpIdx].size();
    for (size_t i=0; i<potShapesSize; i++)
    {
      const Shape* currShp = (*_shapesRef)[_potShp[potShpIdx][i]];
      if (currShp->shadowHit(ray, *t) && *t < tTmp)
      {
        isCloser |= true;
        shpIdxTmp = _potShp[potShpIdx][i];
        tTmp = *t;
      }
    }

    if (isCloser == true)
    {
      shpIdx = shpIdxTmp;
      *t = tTmp;

      return true;
    }


    return false;
  }


  // Die Wurzel ist ein innerer Knoten
  const uint8_t tStackSize = 64;
  TraversalStack tStack[tStackSize];
  const BIHNode* currNode, * nearNode, * farNode;
  int axis = _root.getAxis();
  int treeIdx = _root.getIndex();
  float nearClip, farClip;
  uint8_t stackPos = 0;
  FLOATING invDir[3] = { 1./ray.d().x, 1./ray.d().y, 1./ray.d().z};
  FLOATING ray_d[3] = {ray.d().x, ray.d().y, ray.d().z};
  FLOATING ray_o[3] = {ray.o().x, ray.o().y, ray.o().z};

  // Die Strahlrichtung entscheidet über near und far node.
  if (ray_d[axis] > 0)
  {
    nearNode = &_bihTree[treeIdx][0];
    nearClip = static_cast<float>(
      (_root.Clip[0] - ray_o[axis]) * invDir[axis] );
    farNode  = &_bihTree[treeIdx][1];
    farClip  = 
      static_cast<float>((_root.Clip[1] - ray_o[axis]) * invDir[axis]);
  }
  else if (ray_d[axis] < 0)
  {
    nearNode = &_bihTree[treeIdx][1];
    nearClip = 
      static_cast<float>((_root.Clip[1] - ray_o[axis]) * invDir[axis]);
    farNode  = &_bihTree[treeIdx][0];
    farClip  = 
      static_cast<float>((_root.Clip[0] - ray_o[axis]) * invDir[axis]);
  }
  // Spezialfall: nearClip und farClip werden inf sein. Der 2. if case
  // wird true liefern, also setze nearNode entsprechend richtig.
  else
  {
    nearClip = -std::numeric_limits<float>::max();
    farClip = std::numeric_limits<float>::max();

    if (ray_o[axis] <= _root.Clip[0])
    {
      farNode  = &_bihTree[treeIdx][0];
      farClip  = tmin;
    }
    if (ray_o[axis] >= _root.Clip[1])
    {
      nearNode  = &_bihTree[treeIdx][1];
      nearClip  = tmax;
    }
  }


  // FarNode muss zuerst in den Stack, wenn überhaupt...
  if (farClip <= tmax)
  {
    tStack[stackPos].node = farNode;
    tStack[stackPos].tmin = std::max(tmin, farClip);
    tStack[stackPos].tmax = tmax;
    stackPos++;
  }

  // Jetzt wird geschaut ob nearNode in den Stack muss
  if (nearClip >= tmin && nearClip >= 0)
  {
    tStack[stackPos].node = nearNode;
    tStack[stackPos].tmin = tmin;
    tStack[stackPos].tmax = std::min(tmax, nearClip);
    stackPos++;
  }


  while (stackPos > 0)
  {
    --stackPos;
    currNode = tStack[stackPos].node;
    tmin     = tStack[stackPos].tmin;
    tmax     = tStack[stackPos].tmax;

    // Frühe Abbruch, falls ein bereits gefundener Schnitt näher ist als tmin
    // des aktuellen Knotens
    if (tmin > tTmp)
      continue;

    // Überprüfen ob der Knoten ein Blatt ist
    if (currNode->isLeaf() )
    {
      /*if (currNode->items <= 0)
        continue;*/

      size_t potShpIdx = currNode->getIndex();
      size_t potShapesSize = _potShp[potShpIdx].size();
      for (size_t i=0; i<potShapesSize; i++)
      {
        const Shape* currShp = (*_shapesRef)[_potShp[potShpIdx][i]];
        if (currShp->shadowHit(ray, *t) && *t < tTmp)
        {
          isCloser |= true;
          shpIdxTmp = _potShp[potShpIdx][i];
          tTmp = *t;
        }
      }

      continue;
    }


    // Die Strahlrichtung entscheidet über near und far node.
    axis    = currNode->getAxis();
    treeIdx = currNode->getIndex();
    if (ray_d[axis] > 0)
    {
      nearNode = &_bihTree[treeIdx][0];
      nearClip = 
        static_cast<float>((currNode->Clip[0] - ray_o[axis]) * invDir[axis]);
      farNode  = &_bihTree[treeIdx][1];
      farClip  = 
        static_cast<float>((currNode->Clip[1] - ray_o[axis]) * invDir[axis]);
    }
    else if (ray_d[axis] < 0)
    {
      nearNode = &_bihTree[treeIdx][1];
      nearClip = 
        static_cast<float>((currNode->Clip[1] - ray_o[axis]) * invDir[axis]);
      farNode  = &_bihTree[treeIdx][0];
      farClip  = 
        static_cast<float>((currNode->Clip[0] - ray_o[axis]) * invDir[axis]);
    }
    // Spezialfall: nearClip und farClip werden inf sein. Der 2. if case
    // wird true liefern, also setze nearNode entsprechend richtig.
    else
    {
      nearClip = -std::numeric_limits<float>::max();
      farClip = std::numeric_limits<float>::max();

      if (ray_o[axis] <= currNode->Clip[0])
      {
        farNode  = &_bihTree[treeIdx][0];
        farClip  = tmin;
      }
      if (ray_o[axis] >= currNode->Clip[1])
      {
        nearNode  = &_bihTree[treeIdx][1];
        nearClip  = tmax;
      }

    }


    // FarNode muss zuerst in den Stack, wenn überhaupt...
    if (farClip <= tmax)
    {
      tStack[stackPos].node = farNode;
      tStack[stackPos].tmin = std::max(tmin, farClip);
      tStack[stackPos].tmax = tmax;
      stackPos++;
    }

    // Jetzt wird geschaut ob nearNode in den Stack muss
    if (nearClip >= tmin && nearClip >= 0)
    {
      tStack[stackPos].node = nearNode;
      tStack[stackPos].tmin = tmin;
      tStack[stackPos].tmax = std::min(tmax, nearClip);
      stackPos++;
    }

  }


  if (isCloser == true)
  {
    shpIdx = shpIdxTmp;
    *t = tTmp;

    return true;
  }
  else
    return false;

}


//_____________________________________________________________________________
void
BIH::getSplitAxis(/*const GetSplitAxisParam& gsap,*/
                  const AABB& pAABB, AABB& lAABB, AABB& rAABB,
                  int& axis,
                  float& axisValue)
{
 /* const std::vector<std::pair<size_t, float> >& xSortShp = gsap.xSortShp;
  const std::vector<std::pair<size_t, float> >& ySortShp = gsap.ySortShp;
  const std::vector<std::pair<size_t, float> >& zSortShp = gsap.zSortShp;
  const std::array<size_t, 2>& xPartitionIds = gsap.xPartitionIds;
  const std::array<size_t, 2>& yPartitionIds = gsap.yPartitionIds;
  const std::array<size_t, 2>& zPartitionIds = gsap.zPartitionIds;
  const AABB& pAABB = gsap.pAABB;*/


  float xRange = pAABB.maxSlab[0] - pAABB.minSlab[0];
  float yRange = pAABB.maxSlab[1] - pAABB.minSlab[1];
  float zRange = pAABB.maxSlab[2] - pAABB.minSlab[2];
  float maxRange = std::max(xRange, std::max(yRange, zRange) );

  if (xRange == maxRange)
    axis = 0;
  else if (yRange == maxRange)
    axis = 1;
  else
    axis = 2;

//#ifdef SPLIT_EVEN
//  switch (axis)
//  {
//  case 0:
//    {
//      size_t offset    = (xPartitionIds[1] - xPartitionIds[0]) / 2;
//      axisValue        = xSortShp[xPartitionIds[0] + offset + 1].second;
//    
//      break;
//    }
//  case 1:
//    {
//      size_t offset    = (yPartitionIds[1] - yPartitionIds[0]) / 2;
//      axisValue        = ySortShp[yPartitionIds[0] + offset + 1].second;
//
//      break;
//    }
//  case 2:
//    {
//      size_t offset    = (zPartitionIds[1] - zPartitionIds[0]) / 2;
//      axisValue        = zSortShp[zPartitionIds[0] + offset + 1].second;
//    }
//    break;
//  }
//
//  lAABB = rAABB = pAABB;
//
//  lAABB.maxSlab[axis] = axisValue;
//  rAABB.minSlab[axis] = axisValue;
//#else
  axisValue = static_cast<float>(pAABB.getCentroid()[axis]);
  lAABB = rAABB = pAABB;

  lAABB.maxSlab[axis] = axisValue;
  rAABB.minSlab[axis] = axisValue;
//#endif // SPLIT_EVEN
}
