#pragma once

#ifndef BIH_H_
#define BIH_H_

#include <algorithm>
#include <list>
#include <set>
#include <vector>

#include "aabb.h"
#include "accelerator.h"
#include "ray.h"


// Forward Deklarationen
class ShadeRec;
struct GetSplitAxisParam;
struct ClipAndPartitionParam;

//_____________________________________________________________________________

struct BIHNode
{
  //Konstruktor für leeren inneren Knoten
  BIHNode() : _index(0){}
  //Konstruktor für inneren Knoten
  BIHNode(int index, int axis, float clip[2]);
  //Konstruktor für Blätter
  BIHNode(int shpIndex, int nShp);
  ~BIHNode(){}

  inline void setAxis(int axis); // x = 0; y = 1, z = 2
  inline int  getAxis() const;
  inline void setIndex(int index);
  inline int  getIndex() const;
  inline void setToLeaf();
  inline bool isLeaf() const;
  
  union
  {
    int items;      //leaf only
    float Clip[2];  //internal node only
  };

private:
  int _index;  //lowest bits: axis (00, 01, 10) or leaf(11)
};


//_____________________________________________________________________________
class BIH : public Accelerator
{
public:
  BIH(void);
  BIH(const std::vector<Shape*>& shapes);

  virtual ~BIH(void);

  virtual bool create();
  virtual bool create(int maxDepth, int nShpPerLeaf);
  virtual Accelerator* clone() const;
  virtual Accelerator* newInstance() const;

  virtual bool hit(const Ray& ray, size_t& shpIdx, double* tmin=NULL) const;
  virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
  virtual bool shadowHit(const Ray& ray, double& tmin) const;


protected:

  virtual bool hitCalc(const Ray& ray, size_t& shpIdx, double* tmin=NULL) 
    const;
  void getSplitAxis(/*const GetSplitAxisParam& gsap,*/
                    const AABB& pAABB, AABB& lAABB,
                    AABB& rAABB, int& axis, float& axisValue);
  void clipAndPartition(const ClipAndPartitionParam& capp,
                        std::array<size_t, 2>& lPartitionIds,
                        std::array<size_t, 2>& rPartitionIds,
                        std::list<std::pair<size_t, Point> >& lShpIds,
                        std::list<std::pair<size_t, Point> >& rShpIds);
 

  AABB _globalAABB;
  BIHNode _root;
  std::vector<std::array<BIHNode, 2> > _bihTree;
  std::vector<std::vector<size_t> > _potShp;
  
};


#include "bih.inl"

#endif //BIH_H_