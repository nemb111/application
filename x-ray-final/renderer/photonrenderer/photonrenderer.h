#pragma once

#ifndef PHOTONRENDERER_H_
#define PHOTONRENDERER_H_

#include <memory>

#include "hemispheres.h"
#include "kdtree.h"
#include "options.h"
#include "photon.h"
#include "photonpool.h"
#include "prendfilter.h"
#include "renderer.h"


//Forward Deklarationen
class KDTree;


namespace PREND
{
  struct Geom
  {
    enum GeomType {DISC, CHULL};
    
    GeomType geomType;

    // Einstellungen die nur Disc betreffen
    bool useDensEst;
    float diskHeight;  // Die maximale Höhe die ein Photon von der "Konsolidierungsscheibe" abweichen darf

    // Einstellungen die nur die konvexe Hülle betreffen
    bool useBHeuristic;
    float chullHeight;


    Geom()
      : geomType(DISC), useDensEst(false), diskHeight(0.1f),
        useBHeuristic(false), chullHeight(0.1f)
    {}

  };


  struct Params
  {
    bool disable;  // Schaltet die Verwendung der Photonmap ein und aus
    bool useRadiusSearch; // Suche entweder nach n nächsten Nachbarn oder verwende eine Distanz (Radius)
    size_t knn;           // Anzahl der nächsten Nachbarn die gefunden werden sollen
    size_t minKPhot; // Mindestanzahl der Photonen die benötigt werden um Beleuchtung zu berechnen
    float radius;      
   
    Geom geom;
    
    PRF::Modus filterModus; // Welcher Filter soll verwendet werden. Keiner, Kegel-Filter oder Gauß-Filter
    PRF::Params filterParams; // Setzt Parameter die in den jeweiligen Filter verwendet werden

    flann::SearchParams flannSearchParams;


    Params() :
      disable(false),
      useRadiusSearch(false), knn(100), minKPhot(20), radius(1.f),
      filterModus(PRF::NO_FILTER), filterParams(PRF::Params() )
    {
      flannSearchParams.checks = flann::FLANN_CHECKS_UNLIMITED;
      flannSearchParams.cores  = 1;
      flannSearchParams.sorted = true;
    }

  };

}


//_____________________________________________________________________________
class PhotonRenderer : public Renderer
{
public:
  PhotonRenderer(void);
  /**
   * \param atLeastNCPhot Mindestanzahl an caustics-Photonen.
   * \param atLeastNGPhot Mindestanzahl an global-Photonen.
   * \param maxDepth Maximale Anzahl an Reflexionen eines Photons.
   * \param hemisphereS Ein von der Lichtquelle benutzter Hemisphären-Sampler
   * \param nSamplePattern Anzahl unterschiedlicher Sample-mengen (über Hemisphäre)
   */
  PhotonRenderer(uint atLeastNCPhot, uint atLeastNGPhot, uint maxDepth,
                 HemisphereS* hemisphereS, uint nSamplePattern=100);
  virtual ~PhotonRenderer(void);

  virtual void init(const World* world);

  virtual bool Render(const SampleList& sampleList, uint iteration) const;
  /**
   * Rekursion wird in diesem Renderer nicht verwendet.
   * \param depth Gibt die Rekursionstiefe an für die reflektierte und gebrochene rays berechnet werden.
   * \param contribution Gibt den prozentualen Anteil eines Ray bei der Berechnung des terminalen Farbwertes an.
   */
   virtual Spectrum Li(const Ray& ray, const int depth,
                      float contribution=1.f) const;

  virtual void debugCaustics(bool debugCaustics, float dCShereRadius=0.2f,
    std::function<bool(const TracePhoton& tracePhoton)> causticsComp=nullptr);
  virtual void debugGlobal(bool debugGlobal, float dGSphereRadius=0.2f,
    std::function<bool(const TracePhoton& tracePhoton)> globalComp=nullptr);

  virtual void setIndexParmas(const flann::IndexParams& indexParams);
  virtual void setCParams(const PREND::Params& cParams);
  virtual void setGParams(const PREND::Params& gParams);

protected:

  // Testet welche Photonen sich innerhalb der Geometrie befinden und gibt den
  // Flächeninhalt zurück. Zur Auswahl stehen: Kreis, konvexe Hülle
  static float pInGeometry(const std::vector<std::vector<size_t> >& indices,
                           const std::vector<std::vector<
                           flann::L2<FLOATING>::ResultType > >& dists,
                           const std::vector<UPCPhoton>& photVec,
                           const ShadeRec& shadeRec,
                           const PREND::Params& params,
                           std::list<std::pair<
                             size_t, flann::L2<FLOATING>::ResultType > >&
                             photonsInGeometry,
                           float& pMaxDist);


  static Spectrum photonContribution(const ShadeRec& shadeRec, const KDTree& kdTree,
                                     const PREND::Params& params,
                                     const std::vector<UPCPhoton>& photVec);


  void createPhotons(
    std::vector<std::pair<size_t, UPTPhoton> >& causticsPhotonVec,
    std::vector<std::pair<size_t, UPTPhoton> >& globalPhotonVec);
  // Gibt false zurück wenn photonTraceVec.size() >= _nPhotons. Füllt
  // photonTraceVec mit getraceten Photonen und ordnet Ihnen einen Lichtquellen
  // Index zu.
  bool tracePhotons(const std::vector<UPTPhoton>& photonVec,
                    std::vector<std::pair<size_t, UPTPhoton> >& photonTraceVec,
                    size_t lightIdx, PhotonPool& photonPool);
  // true wenn Photon absorbiert wurde, false wenn es die Szene verlassen hat
  bool tracePhoton(const TracePhoton& curPhoton, TracePhoton& tracedPhoton,
                   int depth);

  
  // Anteil kaustischer Photonen an indirekter Beleuchtung
  Spectrum cPhotonContribution(const ShadeRec& shadeRec) const;
  // Anteil globaler Photonen an indirekter Beleuchtung
  Spectrum gPhotonContribution(const ShadeRec& shadeRec) const;

  

  
  uint _atLeastNCPhot;
  uint _atLeastNGPhot;
  uint _maxDepth;
  uint _nSamplePattern;
  
  
  flann::IndexParams  _indexParams;
  PREND::Params _cParams;
  PREND::Params _gParmas;

 
  HemisphereS* _hemisphereS;
  KDTree*      _causticsKDTree;
  KDTree*      _globalKDTree;
  UnitSampler* _unitSampler;

  std::vector<UPCPhoton > _causticsPhotonVec;
  std::vector<UPCPhoton > _globalPhotonVec;
  

  /*+++++++++++++++++++++++++ Debugging Stuff +++++++++++++++++++++++++++++++*/
  bool _debugCaustics;
  bool _debugGlobal;
  float _dCSphereRadius;
  float _dGSphereRadius;
  Accelerator* _debugAccelerator;
  std::vector<Shape* > _debugShapeVec;
  // Folgende Funktoren geben true zurück wenn ein gewisses Kriterium erfüllt ist.
  // Photonen werden im Debug-Modus nur visualisiert wenn der Funktor true liefert.
  std::function<bool(const TracePhoton& tracePhoton)> _causticsComp;
  std::function<bool(const TracePhoton& tracePhoton)> _globalComp;
  /*-------------------------------------------------------------------------*/


};


#endif //PHOTONRENDERER_H_