#include "stdafx.h"


#include <boost/geometry.hpp>
#include <boost/geometry/geometries/multi_point.hpp> 
#include <boost/geometry/geometries/register/point.hpp>


//#include <random>

//#include "arealight.h"
//#include "debugmaterial.h"
//#include "hammersleys.h"
//#include "kdtree.h"
//#include "mutexedrng.h"
//#include "options.h"
//#include "parser.h"
//#include "photon.h"
//#include "photonrenderer.h"
//#include "prendfilter.h"
//#include "sphere.h"
//#include "stratifieds.h"
//#include "threadlocal.h"



/***************** Eigene Punktdefinition für Boost Geometry *****************/
struct CHullP
{
   CHullP(double x = 0, double y = 0, size_t id=0)
    : x(x), y(y), id(id) {}


double x, y;
size_t id;
};
// Eigene Punktdefinition registrieren
BOOST_GEOMETRY_REGISTER_POINT_2D(CHullP, double, boost::geometry::cs::cartesian, x, y)



//_____________________________________________________________________________
PhotonRenderer::PhotonRenderer(void)
  : Renderer(), _debugCaustics(false), _debugGlobal(false),
    _atLeastNCPhot(0), _atLeastNGPhot(10000), _maxDepth(3),
    _nSamplePattern(100), _dCSphereRadius(0.2f), _dGSphereRadius(0.2f),
    _indexParams(flann::KDTreeSingleIndexParams(10) ),
    _debugAccelerator(nullptr), _causticsKDTree(nullptr),
    _globalKDTree(nullptr)
{
  _unitSampler = new StratifiedS(100);
  _hemisphereS = new HemisphereS(_unitSampler, 1);
  _globalPhotonVec.reserve(_atLeastNGPhot);


}


//_____________________________________________________________________________
PhotonRenderer::PhotonRenderer(
  uint atLeastNCPhot, uint atLeastNGPhot, uint maxDepth,
  HemisphereS* hemisphereS, uint nSamplePattern/*=100*/)
  : Renderer(), _debugCaustics(false), _debugGlobal(false),
    _atLeastNCPhot(atLeastNCPhot), _atLeastNGPhot(atLeastNGPhot),
    _maxDepth(maxDepth),
    _nSamplePattern(nSamplePattern), _dCSphereRadius(0.2f),
    _dGSphereRadius(0.2f),
    _indexParams(flann::KDTreeSingleIndexParams(10) ),
    _debugAccelerator(nullptr),
    _hemisphereS(hemisphereS), _causticsKDTree(nullptr),
    _globalKDTree(nullptr), _unitSampler(nullptr)
{
  _causticsPhotonVec.reserve(_atLeastNCPhot);
  _globalPhotonVec.reserve(_atLeastNGPhot);

}


//_____________________________________________________________________________
PhotonRenderer::~PhotonRenderer(void)
{
  delete _unitSampler;
  _unitSampler = nullptr;
  delete _debugAccelerator;
  _debugAccelerator = nullptr;
  delete _causticsKDTree;
  _causticsKDTree = nullptr;
  delete _globalKDTree;
  _globalKDTree = nullptr;

  for (auto it=_debugShapeVec.begin(); it!=_debugShapeVec.end(); it++)
    delete (*it);
}


//_____________________________________________________________________________
void
PhotonRenderer::init(const World* world)
{
  assert(world);
  Renderer::init(world);

  std::vector<std::pair<size_t, UPTPhoton> > causticsPhotonVec;
  std::vector<std::pair<size_t, UPTPhoton> > globalPhotonVec;

  // Der Photonrenderer kann nur mit Arealights verwendet werden
  size_t nLights = _world->lights.size();
  for (size_t i=0; i<nLights; i++)
    if (nullptr == dynamic_cast<AreaLight*>(_world->lights[i] ) )
    {
      EOUT("PhotonRenderer kann nur mit AreaLights verwendet werden. "
           "Programm wird jetzt beendet.");
      std::cin.get();
      exit(EXIT_FAILURE);
    }

  IOUT("Erzeuge photon map.");
  createPhotons(causticsPhotonVec, globalPhotonVec);


  if (_debugCaustics == true)
  {
    // Es soll die debug-Ansicht gezeigt werden bereite den 
    // _degubShapeVec Vector vor
    for (auto cit=causticsPhotonVec.cbegin(); cit!=causticsPhotonVec.cend();
         cit++)
    {
      if (_causticsComp(*(cit->second.get() ) ) == true)
      {

        Sphere* sphere = new Sphere((*cit).second->photonRay.o(),
                                    _dCSphereRadius);
        Spectrum color = (*cit).second->color * (*cit).second->intensity;
        if (color.maxCoeff() > 1.f )
          color /= color.maxCoeff();
        DebugMaterial* debugMaterial = 
            new DebugMaterial(color);
        Parser::GB.mrkDel<Material*>(debugMaterial);

        sphere->setMaterial(debugMaterial);
        sphere->setThrowsShadow(false);

 
        _debugShapeVec.push_back(sphere );

      }
    }
  }
  if (_debugGlobal == true)
  {
    // Es soll die debug-Ansicht gezeigt werden bereite den 
    // _degubShapeVec Vector vor
    for (auto cit=globalPhotonVec.cbegin(); cit!=globalPhotonVec.cend(); cit++)
    {
      if (_globalComp(*(cit->second.get() ) ) == true)
      {

        Sphere* sphere = new Sphere((*cit).second->photonRay.o(),
                                    _dGSphereRadius);
        Spectrum color = (*cit).second->color * (*cit).second->intensity;
        if (color.maxCoeff() > 1.f )
          color /= color.maxCoeff();
        DebugMaterial* debugMaterial = 
            new DebugMaterial(color);
        Parser::GB.mrkDel<Material*>(debugMaterial);

        sphere->setMaterial(debugMaterial);
        sphere->setThrowsShadow(false);

 
        _debugShapeVec.push_back(sphere );

      }
    }
  }
  if (_debugCaustics || _debugGlobal)
  {
    if (_debugShapeVec.size() <= 0)
    {
      EOUT("Es befinden sich keine Objekte im \"_debugShapeVec\".");
      std::cin.get();
      exit(EXIT_FAILURE);
    }

    _debugAccelerator = _world->accelerator->newInstance();
    _debugAccelerator->addShapes(_debugShapeVec);
    _debugAccelerator->create();
  }



  _causticsPhotonVec.reserve(causticsPhotonVec.size() );
  _globalPhotonVec.reserve(globalPhotonVec.size() );


  // Die TracePhotonen umwandeln zum endgültigen Speichern.
  for (auto rit=causticsPhotonVec.rbegin(); rit!=causticsPhotonVec.rend(); 
       rit++)
  {
    Photon* photon = new Photon;
    photon->location = (*rit).second->photonRay.o();
    photon->wi       = (*rit).second->photonRay.d();
    photon->color    = (*rit).second->color * (*rit).second->intensity;


    _causticsPhotonVec.push_back(UPCPhoton(photon) );
    causticsPhotonVec.pop_back();
  }
  
  for (auto rit=globalPhotonVec.rbegin(); rit!=globalPhotonVec.rend(); 
       rit++)
  {
    Photon* photon = new Photon;
    photon->location = (*rit).second->photonRay.o();
    photon->wi       = (*rit).second->photonRay.d();
    photon->color    = (*rit).second->color * (*rit).second->intensity;


    _globalPhotonVec.push_back(UPCPhoton(photon) );
    globalPhotonVec.pop_back();
  }


  if (_causticsPhotonVec.size() > 0)
    _causticsKDTree = new KDTree(_causticsPhotonVec, _indexParams);
  if (_globalPhotonVec.size() > 0)
    _globalKDTree = new KDTree(_globalPhotonVec, _indexParams);


  if (_causticsPhotonVec.size() == 0 && _globalPhotonVec.size() == 0)
  {
    EOUT("Es konnte weder eine caustics- noch global-Photonmap erstellt werden.");
    std::cin.get();
    exit(EXIT_FAILURE);
  }

}


//_____________________________________________________________________________
bool
PhotonRenderer::Render(const SampleList& sampleList, uint iteration) const
{
  if (iteration > 1)
    return false;


  Spectrum resSpec(BLACK);

  for (size_t i=0; i<sampleList.size(); i++)
  {
    // Bedingung die Überprüft, ob nur ein einziger Pixel gerendert werden soll
    if (_debugPixelSet && !(sampleList[i].pxlIdxX == _debugPixelX &&
        sampleList[i].pxlIdxY == _debugPixelY) )
      continue;

    resSpec = BLACK;
    for (size_t j=0; j<sampleList[i].rays.size(); j++)
    {
      resSpec += Li(sampleList[i].rays[j], 0, 1.0f) * sampleList[i].weigths[j];

    }
    
    writeBackGlobIllSample(sampleList[i].pxlIdxX, sampleList[i].pxlIdxY, resSpec);
  }

  return true;
}


//_____________________________________________________________________________
Spectrum
PhotonRenderer::Li(const Ray& ray, const int depth, float contribution/*=1.f*/)
const
{
  // Zeige die Debugansicht
  if (_debugCaustics || _debugGlobal)
  {
    assert(_debugAccelerator != NULL);
    assert(_debugShapeVec.size() > 0);
    assert(depth >= 0);
    assert(contribution >= 0 && contribution <= 1.f);

    double tmin = std::numeric_limits<double>::max();
    ShadeRec shadeRec(*_world);

    bool hit =_debugAccelerator->hit(ray, tmin, shadeRec);

    if (hit)
    {
      shadeRec.ray = ray;
      shadeRec.t   = tmin;
      return shadeRec.material->areaLightShade(shadeRec);
    }
    else
    {
      return BLACK;
    }
  }
  // Rendere normal
  else
  {
    assert(_world->accelerator != nullptr);
    assert(depth >= 0);
    assert(contribution >= 0 && contribution <= 1.f);

    double tmin = std::numeric_limits<double>::max();
    ShadeRec shadeRec(*_world);
    shadeRec.depth = depth;
    shadeRec.contribution = contribution;

    bool hit =_world->accelerator->hit(ray, tmin, shadeRec);

    if (hit)
    {
      Spectrum Li = BLACK;
      shadeRec.ray = ray;
      shadeRec.t   = tmin;
      
      if (_cParams.disable == false)
        Li += cPhotonContribution(shadeRec);
      if (_gParmas.disable == false)
        Li += gPhotonContribution(shadeRec);


      return Li;
    }
    else
    {
      return BLACK;
    }
  }


  return BLACK;
}


//_____________________________________________________________________________
void
PhotonRenderer::debugCaustics(bool debugCaustics, float dCSphereRadius/*=0.2f*/,
  std::function<bool(const TracePhoton& tracePhoton)> causticsComp/*=nullptr*/)
{
  _debugCaustics = debugCaustics;
  _dCSphereRadius = dCSphereRadius;
  if (causticsComp == nullptr)
    _causticsComp = [](const TracePhoton& tracePhoton) -> bool { return true; };
  else
    _causticsComp = causticsComp;
}


//_____________________________________________________________________________
void
PhotonRenderer::debugGlobal(bool debugGlobal, float dGSphereRadius/*=0.2f*/,
    std::function<bool(const TracePhoton& tracePhoton)> globalComp/*=nullptr*/)
{
  _debugGlobal = debugGlobal;
  _dGSphereRadius = dGSphereRadius;
  if (globalComp == nullptr)
    _globalComp = [](const TracePhoton& tracePhoton) -> bool {return true;};
  else
    _globalComp = globalComp;
}


//_____________________________________________________________________________
void
PhotonRenderer::setIndexParmas(const flann::IndexParams& indexParams)
{
  _indexParams = indexParams;
}


//_____________________________________________________________________________
void
PhotonRenderer::setCParams(const PREND::Params& cParams)
{
  _cParams = cParams;
}


//_____________________________________________________________________________
void
PhotonRenderer::setGParams(const PREND::Params& gParams)
{
  _gParmas = gParams;
}


//_____________________________________________________________________________
void
PhotonRenderer::createPhotons(
  std::vector<std::pair<size_t, UPTPhoton> >& causticsPhotonVec,
  std::vector<std::pair<size_t, UPTPhoton> >& globalPhotonVec)
{
  causticsPhotonVec.reserve(_atLeastNGPhot);
  globalPhotonVec.reserve(_atLeastNGPhot);
  std::vector<size_t> countPhotonPerLight(_world->lights.size(), 0);


  omp_set_num_threads(Options::getInstance().nCores);

#pragma omp parallel shared(countPhotonPerLight) shared(causticsPhotonVec) \
                     shared(globalPhotonVec)
{
  uint causticsPhotonVecSize = 0;
  uint globalPhotonVecSize = 0;
  const AreaLight* currLight = nullptr;
  PhotonPool photonPool((_atLeastNCPhot + _atLeastNGPhot) );
  // Beinhaltet den Index des zugehörigen Lichts und einen pointer auf das
  // Photon
  std::vector<std::pair<size_t, UPTPhoton > > photonTraceVec;
  std::mt19937 generator(
    static_cast<unsigned long>(Options::getInstance().seed) );
  std::uniform_int_distribution<size_t> lightDist(0, _world->lights.size() -1);
  ShadeRec dummy(*_world);
 

  while (causticsPhotonVecSize < _atLeastNCPhot ||
         globalPhotonVecSize < _atLeastNGPhot)
  {
    size_t lightIdx = MutexedRNG::number(lightDist);
    currLight = dynamic_cast<AreaLight*>(_world->lights[lightIdx]);
    const std::vector<const Point>& samplePoints =
      currLight->getSamplePoints(dummy);
    std::vector<UPTPhoton > startPhotVec;

    for (auto it = samplePoints.cbegin(); it!=samplePoints.cend(); it++)
    {
      startPhotVec = currLight->getStartPhotons(
          *it, photonPool, _hemisphereS, _nSamplePattern);
      #pragma omp critical(countPhotonPerLight)
      {
      countPhotonPerLight[lightIdx] += startPhotVec.size();

      }


      // Die auf dem Licht erzeugten Photonen in die Szene schießen.
      tracePhotons(startPhotVec, photonTraceVec, lightIdx, photonPool);

      // Absorbierte Photonen entweder in die caustics- oder global-Map stecken
      for (auto it=photonTraceVec.begin(); it!=photonTraceVec.end(); it++)
      {
        if (it->second->onlySpecularRefl == true)
        {
          #pragma omp critical(causticsPhotonVec)
          {
          causticsPhotonVec.push_back(
            std::make_pair(std::move(it->first), std::move(it->second) ) );
          }
        }
        else
          #pragma omp critical(globalPhotonVec)
          {
          globalPhotonVec.push_back(
            std::make_pair(std::move(it->first), std::move(it->second) ) );
          }
      }
      #pragma omp critical(causticsPhotonVec)
      {
        causticsPhotonVecSize = static_cast<uint>(causticsPhotonVec.size() );
      }
      #pragma omp critical(globalPhotonVec)
      {
        globalPhotonVecSize = static_cast<uint>(globalPhotonVec.size() );
      }
      photonTraceVec.clear();

      #pragma omp master
      {
      // Zeige eine Progressbar
      if (_atLeastNCPhot / (causticsPhotonVecSize + 1) >  // +1 um zero division zu verhindern
          _atLeastNGPhot / (globalPhotonVecSize + 1) )
      {
        progressbar(causticsPhotonVecSize, _atLeastNCPhot);
      }
      else
      {
        progressbar(globalPhotonVecSize, _atLeastNGPhot);
      }
      }
        

      // Die Startphotonen wieder an den Pool zurückgeben
      while (startPhotVec.size() )
      {
        photonPool.push(std::move(startPhotVec.back() ) );
        startPhotVec.pop_back();
      }

    }
  }


  photonPool.clear();
} // #pragma omp parallel


  // Nochmal alle Photonen durchgehen und die Energie entsprechend der
  // Anzahl erstellter Photonen anpassen
  size_t cVecSize = causticsPhotonVec.size();
  size_t gVecSize = globalPhotonVec.size();
  #pragma omp parallel for
  for (int i=0; i<cVecSize; i++)
  {
    causticsPhotonVec[i].second->intensity /= 
      countPhotonPerLight[causticsPhotonVec[i].first];
  }
  #pragma omp parallel for
  for (int i=0; i<gVecSize; i++)
  {
    globalPhotonVec[i].second->intensity /=
      countPhotonPerLight[globalPhotonVec[i].first];
  }



}


//_____________________________________________________________________________
bool
PhotonRenderer::tracePhotons(
  const std::vector<UPTPhoton>& photonVec,
  std::vector<std::pair<size_t, UPTPhoton> >& photonTraceVec,
  size_t lightIdx, PhotonPool& photonPool)
{
  UPTPhoton tracedPhoton;

  for (auto it=photonVec.begin(); it!=photonVec.end(); it++)
  {
   /* #pragma omp critical(_photonPool)
    {*/
    tracedPhoton = photonPool.pop();
    //}
    *tracedPhoton = *(*it);
    // Gibt true zurück wenn das Photon absobiert wurde, false falls das 
    // Photon die Szene verlassen hat.
    if (tracePhoton(**it, *tracedPhoton, 0)  == true)
    {
      photonTraceVec.push_back(
      std::make_pair<size_t, UPTPhoton>(std::move(lightIdx),
                                        std::move(tracedPhoton) ) );
    }
    else
    {
    /*  #pragma omp critical(_photonPool)
      {*/
      photonPool.push(std::move(tracedPhoton) );
      //}
    }

  }




  return true;
}


//_____________________________________________________________________________
bool
PhotonRenderer::tracePhoton(const TracePhoton& curPhoton, 
                            TracePhoton& tracedPhoton,
                            int depth)
{
  assert(_world->accelerator != NULL);
  assert(_world->shapes.size() > 0);
  assert(depth >= 0);

  if (static_cast<uint>(depth) > _maxDepth)
    return false;
  // Photonen die keinen Strahlungsfluss besitzen müssen nicht weiter
  // betrachtet werden
  if ((curPhoton.color * curPhoton.intensity).magnitude() <= 0.)
    return false;

  double tmin = std::numeric_limits<double>::max();
  ShadeRec shadeRec(*_world);
  shadeRec.depth = depth;
  shadeRec.contribution = 1.f;
   
  bool hit =_world->accelerator->hit(curPhoton.photonRay, tmin, shadeRec);



  if (hit)
  {
    bool absorbed;

    shadeRec.ray = curPhoton.photonRay;
    shadeRec.t   = tmin;
    // Gibt ein neues Photon mit veränderter Farbe Position und Richtung
    // zurück.
    absorbed = shadeRec.material->tracePhoton(curPhoton, tracedPhoton,
                                              shadeRec);
    // Die direkte Beleuchtung darf nicht 2x mit einbezogen werden.
    if (absorbed == true && depth > 0)
    {
      tracedPhoton.depth = depth;


      return true;
    }
    else if (absorbed == true && depth <= 0)
      return false;
    else
    {
      TracePhoton tmpPhoton(tracedPhoton);
      return tracePhoton(tmpPhoton, tracedPhoton, shadeRec.depth + 1);
    }

  }
  else
  {
    return false;
  }
  
  
  return false;
}


//_____________________________________________________________________________
float
PhotonRenderer::pInGeometry(const std::vector<std::vector<size_t> >& indices,
                            const std::vector<std::vector<
                              flann::L2<FLOATING>::ResultType > >& dists,
                            const std::vector<UPCPhoton>& photVec,
                            const ShadeRec& shadeRec, 
                            const PREND::Params& params,
                            std::list<std::pair<
                              size_t, flann::L2<FLOATING>::ResultType > >& 
                              photonsInGeometry,
                            float& pMaxDist)
{
  float area = 0;


  // Hier wird die Scheibe behandelt
  if (params.geom.geomType == params.geom.DISC)
  {
    int notInDisc = 0;
    Point barycenter(0,0,0);
    pMaxDist = -std::numeric_limits<float>::max();


     // Testen ob die Photonen in einer Scheibe liegen mit Höhe diskHeight
    for (size_t i=0; i<indices[0].size(); i++)
    {
      const Point& photLoc(photVec[indices[0][i] ]->location);
      Vector hitPntToPhoton = photLoc - shadeRec.hitPoint;

      float heightDiff = static_cast<float>(shadeRec.normal * hitPntToPhoton);

      if (fabs(heightDiff) <= params.geom.diskHeight)
      {
        // Flächenschwerpunkt berechnen
        barycenter = barycenter.baryAdd(photLoc);

        photonsInGeometry.push_back(
          std::make_pair(indices[0][i], dists[0][i]) );
        pMaxDist = static_cast<float>(
          (pMaxDist < dists[0][i]) ? dists[0][i] : pMaxDist); // Es handelt sich hier um die quadrierte Distanz
      }
      else
      {
        notInDisc++;
      }
    }

    area = static_cast<float>(PI * pMaxDist);
    barycenter.normWIP();


    if (params.geom.useDensEst)
    {
      int photInSmallCirc = 0;
      int photToBeInBigCirc = 0;
      float halfR = std::sqrt(pMaxDist) / 2.f;
      float halfRSqr = halfR * halfR;
      float areaRatio = pMaxDist / halfRSqr;
      Vector dirBarycenter = (barycenter - shadeRec.hitPoint).norm();
      Point oSmallCirc = shadeRec.hitPoint + dirBarycenter * halfR;
    
      // Zählen der Photonen, die im kleinen Kreis liegen
      for (auto cit=photonsInGeometry.cbegin(); cit!=photonsInGeometry.cend();
           cit++)
      {
        const Photon& photon = *photVec[ cit->first ];
        if ((photon.location - oSmallCirc).length2() <= halfRSqr )
          photInSmallCirc++;
      }

      photToBeInBigCirc = static_cast<int>(areaRatio * photInSmallCirc);
      area *= photonsInGeometry.size() / static_cast<float>(photToBeInBigCirc);
    }


    return area;
  }
  // Hier wird die konvexe Hülle behandel
  else if (params.geom.geomType == params.geom.CHULL)
  {
    Vector u = shadeRec.normal;
    Vector v(0.34113, 0.12234, -0.9838), w;
    Vector photLocVec;
    Vector hitPointVec(shadeRec.hitPoint.x / shadeRec.hitPoint.w,
                       shadeRec.hitPoint.y / shadeRec.hitPoint.w,
                       shadeRec.hitPoint.z / shadeRec.hitPoint.w);
    boost::geometry::model::multi_point<CHullP> multiPoint;
    boost::geometry::model::polygon<CHullP> cHull;

    std::list<std::pair<size_t, flann::L2<FLOATING>::ResultType > > pInCHull;

    orthNormBasis(u, v, w);


    // Alle Photonen durchgehen und auf Ebene transformieren
    for (size_t i=0; i<indices[0].size(); i++)
    {
      const Point& photLoc(photVec[indices[0][i] ]->location);
      Vector hitPntToPhoton = photLoc - shadeRec.hitPoint;  
      float heightDiff = static_cast<float>(shadeRec.normal * hitPntToPhoton);
      photLocVec = Vector(photLoc.x, photLoc.y, photLoc.z);
      photLocVec *= (1./photLoc.w);


      // Das Photon muss sich auf der richtigen Seite der Geometrie befinden.
      // Und es muss eine gewisse Höhe einhalten
      if (photVec[indices[0][i] ]->wi * shadeRec.normal < 0 &&
          fabs(heightDiff) < params.geom.chullHeight)
      {
        multiPoint.push_back(CHullP(v * photLocVec, w * photLocVec,
                                    i) );
        pInCHull.push_back(
          std::make_pair(indices[0][i], dists[0][i]) );
        pMaxDist = static_cast<float>(
          (pMaxDist < dists[0][i]) ? dists[0][i] : pMaxDist); // Es handelt sich hier um die quadrierte Distanz
      }

    }
    

    boost::geometry::convex_hull(multiPoint, cHull);
    area = static_cast<float>(boost::geometry::area(cHull) );


    // Wenn der Schnittpunkt des Kamerastrahls nicht in der konvexen Hülle lieg
    // erzeugt das Artifakte. Fange diesen Fall hier ab.
    CHullP hitPointCH(v * hitPointVec, w * hitPointVec);
    double comp_dist = boost::geometry::comparable_distance(hitPointCH, cHull);
    if (comp_dist > 0)
    {
      photonsInGeometry.clear();
      return std::numeric_limits<float>::max();     
    }


    // Heuristik nach http://www.cs.princeton.edu/courses/archive/fall02/cs526/papers/course43sig02.pdf
    // um die Energie anzupassen. Es wird nur jedes fünfte Photon auf der
    // konvexen Hülle einbezogen.
    if (params.geom.useBHeuristic &&  pInCHull.size() >= 15)
    {
      std::list<std::pair<size_t, flann::L2<FLOATING>::ResultType > > pOnCHullBoundary;

      for (int i=0; i<cHull.outer().size(); i++)
      {
        if (i % 5 != 0)
          pOnCHullBoundary.push_back(std::make_pair(indices[0][cHull.outer()[i].id],
                                                    dists[0][cHull.outer()[i].id] ) );
      }


      auto comp = [](std::pair<size_t, flann::L2<FLOATING>::ResultType >& first, 
                     std::pair<size_t, flann::L2<FLOATING>::ResultType >& second)
                  {return first.first < second.first;};

    
      pInCHull.sort();
      pOnCHullBoundary.sort();

      std::set_difference(pInCHull.begin(), pInCHull.end(),
                          pOnCHullBoundary.begin(), pOnCHullBoundary.end(),
                          std::back_inserter(photonsInGeometry), 
                          comp);

    }
    else
      std::move(pInCHull.begin(), pInCHull.end(), 
                std::back_inserter(photonsInGeometry) );


    return area;
  }
  else
  {
    WOUT("Es wurde keine korrekte Geometrie zum Erfassen der Photonen "
         "ausgewählt.");

    return 0;
  }

  return 0;
}


//_____________________________________________________________________________
Spectrum
PhotonRenderer::cPhotonContribution(const ShadeRec& shadeRec) const
{
  if (_causticsKDTree == nullptr)
    return BLACK;
  else
    return photonContribution(shadeRec, *_causticsKDTree, _cParams,
                              _causticsPhotonVec);
}


//_____________________________________________________________________________
Spectrum
PhotonRenderer::gPhotonContribution(const ShadeRec& shadeRec) const
{
  if (_globalKDTree == nullptr)
    return BLACK;
  else
    return photonContribution(shadeRec, *_globalKDTree, _gParmas,
                              _globalPhotonVec);
}


//_____________________________________________________________________________
Spectrum
PhotonRenderer::photonContribution(const ShadeRec& shadeRec, 
                                   const KDTree& kdTree,
                                   const PREND::Params& params,
                                   const std::vector<UPCPhoton>& photVec)
{
  float area = 0;
  float pMaxDist = 0; // Die größte Distanz die ein Photon vom Mittelpunkt entfernt ist
  Spectrum res = BLACK;
  FLOATING queryPoint[3] = {shadeRec.hitPoint.x, shadeRec.hitPoint.y,
                            shadeRec.hitPoint.z};
  flann::Matrix<FLOATING> query(queryPoint, 1, 3);
  // Folgende Liste beinhalten den Index sowie zugehörige Distanz
  std::list<std::pair<size_t, flann::L2<FLOATING>::ResultType > > 
    photonsInGeometry;
  std::vector<std::vector<size_t> > indices;
  std::vector<std::vector<flann::L2<FLOATING>::ResultType > > dists;


  if (params.useRadiusSearch == false)
  {
    kdTree.knnSearch(query, indices, dists, params.knn,
                     params.flannSearchParams);
  }
  else
  {
    kdTree.radiusSearch(query, indices, dists, 
                          params.radius * params.radius,
                          params.flannSearchParams);
  }


  // Die folgende Methode bestimmen welche Photonen sich in der angegebenen
  // Geometrie befinden und gibt den Flächeninhalt zurück.
  area = pInGeometry(indices, dists, photVec, shadeRec, params,
                     photonsInGeometry, pMaxDist);
  

  // Weniger als 5 Photonen? Dann macht das ganze nicht wirklich viel Sinn
  if (photonsInGeometry.size() < params.minKPhot)
    return BLACK;


  // Alle Photonen durchgehen und radiance berechnen
  for (auto cit=photonsInGeometry.cbegin(); cit!=photonsInGeometry.cend();
       cit++)
  {
    Spectrum tmpBRDFVal = BLACK;
    const Photon& photon = *photVec[ cit->first ];

   

    tmpBRDFVal = shadeRec.material->photonShade(shadeRec, 
                     photon.wi );

    res += (tmpBRDFVal * photon.color * 
            PRF::weight(params.filterModus, static_cast<float>(pMaxDist),
                        static_cast<float>(cit->second), params.filterParams) );
  }

  
  if (area <= 0)
    return BLACK;
  else
  {
    res = res / area;

    return res;
  }

  return res;

}


