#include "stdafx.h"

//#include "photon.h"
//#include "kdtree.h"



//float* read_points(const char* filename, int rows, int cols)
//{
//	float* data;
//	float *p;
//	FILE* fin;
//	int i,j;
//
//    fin = fopen(filename,"r");
//    if (!fin) {
//        printf("Cannot open input file.\n");
//        exit(1);
//    }
//    
//    data = (float*) malloc(rows*cols*sizeof(float));
//    if (!data) {
//        printf("Cannot allocate memory.\n");
//        exit(1);
//    }
//    p = data;
//    
//    for (i=0;i<rows;++i) {
//        for (j=0;j<cols;++j) {
//            fscanf(fin,"%g ",p);
//            p++;
//        }
//    }
//    
//    fclose(fin);
//    
//    return data;
//}
//
//void write_results(const char* filename, int *data, int rows, int cols)
//{
//	FILE* fout;
//	int* p;
//	int i,j;
//
//    fout = fopen(filename,"w");
//    if (!fout) {
//        printf("Cannot open output file.\n");
//        exit(1);
//    }
//    
//    p = data;
//    for (i=0;i<rows;++i) {
//        for (j=0;j<cols;++j) {
//            fprintf(fout,"%d ",*p);
//            p++;
//        }
//        fprintf(fout,"\n");
//    }
//    fclose(fout);
//}

//_____________________________________________________________________________
KDTree::KDTree(const std::vector<UPCPhoton >& photonVec,
               const flann::IndexParams& indexParams)
  : _mutex(CreateMutex(NULL, FALSE, NULL) ), 
    _index(flann::KDTreeSingleIndexParams(10) )
{
using namespace flann;

  assert(photonVec.size() > 0);

  size_t photonVecSize = photonVec.size();
  std::vector<FLOATING> tmpData(photonVecSize * 3, 0);


  for (int i=0; i<photonVecSize; i++)
  {
    Point tmpPoint = photonVec[i]->location.normW();
    tmpData[i*3] = tmpPoint.x;
    tmpData[i*3 + 1] = tmpPoint.y;
    tmpData[i*3 + 2] = tmpPoint.z;
  }

  
  Matrix<FLOATING> dataset(tmpData.data(), photonVecSize, 3);
  _index = Index<L2<FLOATING> > (dataset, indexParams );
  _index.buildIndex();



  //const int ROWS = 3;
  //const int COLUMNS = 20000000;

  //int nn = 3;

  //std::vector<float> data;
  //data.reserve(COLUMNS * ROWS);

  //for (int i=0; i<COLUMNS; i++)
  //{
  //  for (int j=0; j<ROWS; j++)
  //  {
  //    data[i*ROWS + j] = i;
  //  }
  //}

  //std::vector<float> q;
  //q.reserve(COLUMNS * ROWS);
  //  for (int i=0; i<COLUMNS; i++)
  //{
  //  for (int j=0; j<ROWS; j++)
  //  {
  //    data[i*ROWS + j] = i;
  //  }
  //}



  //Matrix<float> dataset(data.data(), ROWS, COLUMNS);
  //Matrix<float> query(q.data(), ROWS, COLUMNS);

  //Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
  //Matrix<float> dists(new float[query.rows*nn], query.rows, nn);

  //// construct an randomized kd-tree index using 4 kd-trees
  //Index<L2<float> > index(dataset, flann::KDTreeIndexParams(4));
  //index.buildIndex();                                                                                               

  //// do a knn search, using 128 checks
  //index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));


  //delete[] dataset.ptr();
  //delete[] query.ptr();
  //delete[] indices.ptr();
  //delete[] dists.ptr();


}


//_____________________________________________________________________________
KDTree::~KDTree()
{
  CloseHandle(_mutex);
}


//_____________________________________________________________________________
void
KDTree::knnSearch(const flann::Matrix<FLOATING>& queries,
                  std::vector<std::vector<size_t> >& indices,
                  std::vector<
                      std::vector<flann::L2<FLOATING>::ResultType > 
                  >& dists,
                  size_t knn,
                  const flann::SearchParams& params) const
{
//  int mutex_result = WaitForSingleObject(_mutex, //handle to mutex
//                                         INFINITE);    //no time-out interval
//#ifndef NDEBUG
//  switch (mutex_result) 
//  {
//     // The 	acquiring of the mutex failed. Possible problem: mutex not
//    // initialized
//    case WAIT_FAILED:
//    {
//      EOUT("Der Mutex konnte nicht erworben werden");
//      std::cin.get();
//      exit(1);
//    }
//    // The thread got ownership of the mutex
//    case WAIT_OBJECT_0:
//      break; 
//    // The thread got ownership of an abandoned mutex
//    // The database is in an indeterminate state
//    case WAIT_ABANDONED:
//      WOUT("Es wurde ein \"abandoned mutex\" erworben.");
//  }
//#endif //NDEBUG

  _index.knnSearch(queries, indices, dists, knn, params);


//#ifndef NDEBUG
//  if (ReleaseMutex(_mutex) == 0)
//  {
//    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
//    std::cin.get();
//    exit(1);
//  }
//#else
//  ReleaseMutex(_mutex);
//#endif //NDEBUG

}


//_____________________________________________________________________________
void
KDTree::radiusSearch(const flann::Matrix<FLOATING>& queries,
                     std::vector<std::vector<size_t> >& indices,
                     std::vector<
                         std::vector<flann::L2<FLOATING>::ResultType > 
                     >& dists,
                     float radius,
                     const flann::SearchParams& params) const
{
//  int mutex_result = WaitForSingleObject(_mutex, //handle to mutex
//                                         INFINITE);    //no time-out interval
//#ifndef NDEBUG
//  switch (mutex_result) 
//  {
//     // The 	acquiring of the mutex failed. Possible problem: mutex not
//    // initialized
//    case WAIT_FAILED:
//    {
//      EOUT("Der Mutex konnte nicht erworben werden");
//      std::cin.get();
//      exit(1);
//    }
//    // The thread got ownership of the mutex
//    case WAIT_OBJECT_0:
//      break; 
//    // The thread got ownership of an abandoned mutex
//    // The database is in an indeterminate state
//    case WAIT_ABANDONED:
//      WOUT("Es wurde ein \"abandoned mutex\" erworben.");
//  }
//#endif //NDEBUG

  _index.radiusSearch(queries, indices, dists, radius, params);


//#ifndef NDEBUG
//  if (ReleaseMutex(_mutex) == 0)
//  {
//    EOUT("Der Mutex konnte nicht richtig freigegeben werden.");
//    std::cin.get();
//    exit(1);
//  }
//#else
//  ReleaseMutex(_mutex);
//#endif //NDEBUG

}