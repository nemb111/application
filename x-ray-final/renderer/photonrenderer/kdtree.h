#pragma once

#ifndef KDTREE_H_
#define KDTREE_H_

#include <memory>
#include <vector>

#include "flann\flann_no_warn.hpp"
#include "linAlg.h"


// Forward Deklarationen
struct TracePhoton;
struct Photon;


typedef std::unique_ptr<const TracePhoton> UPCTPhoton;
typedef std::unique_ptr<TracePhoton> UPTPhoton;

typedef std::unique_ptr<const Photon> UPCPhoton;



class KDTree
{

private:
  HANDLE _mutex;
  flann::Index<flann::L2<FLOATING> > _index;

public:
  KDTree(const std::vector<UPCPhoton >& photonVec, 
         const flann::IndexParams& indexParams);
  ~KDTree();

  void knnSearch(const flann::Matrix<FLOATING>& queries,
                 std::vector<std::vector<size_t> >& indices,
                 std::vector<
                     std::vector<flann::L2<FLOATING>::ResultType > 
                 >& dists,
                 size_t knn,
                 const flann::SearchParams& params) const;

  void radiusSearch(const flann::Matrix<FLOATING>& queries,
                    std::vector<std::vector<size_t> >& indices,
                    std::vector<
                        std::vector<flann::L2<FLOATING>::ResultType > 
                    >& dists,
                    float radius,
                    const flann::SearchParams& params) const;




};



#endif //KDTREE_H_