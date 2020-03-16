#ifndef COLL_TAPE_STRUCT_H
#define COLL_TAPE_STRUCT_H
#include <cstdlib>
#include <vector>
#include <iostream>
#include "utils.h"

// NTRACK = number of trajectory segments
// MTRACK = number of energy deposition events along the trajectory
// JTRACK = particle type (see (5))
// ETRACK = total energy of the particle (rest + kinetic)
// WTRACK = particle weight
struct first_rec_case_1
{
  int NTRACK, MTRACK, JTRACK;
  float ETRACK, WTRACK;
};
  
// JTRACK = particle type (see (5))
// ETRACK = total energy of the particle (rest + kinetic)
// WTRACK = particle weight
// ICODE = indicates the type of point event giving raise to energy deposition
struct first_rec_case_2
{
  int ZERO, ICODE, JTRACK;
  float ETRACK, WTRACK;
};
// XSCO, YSCO, ZSCO = coordinates of the energy deposition point
// RULL  = amount of energy deposited
struct rec_case_2
{
  float XSCO, YSCO, ZSCO, RULL;
};

// NCASE   = number of primaries treated so far (including current one)
// NPFLKA  = number of particles in the stack
// NSTMAX  = maximum number of particles in stack so far
// TKESUM  = total kinetic energy of the primaries of a user written SOURCE
// WEIPRI  = total weight of the primaries handled so far
struct first_rec_case_3
{
  int NCASE, NPFLKA, NSTMAX;
  float TKESUM, WEIPRI;
};

//    ILOFLK(I)  = type of the Ith stack particle (see (5))
//    ETOT(I) = total energy of Ith stack particle
//    XFLK(I), YFLK(I), ZFLK(I) = source coordinates for the Ith stack particle
//    TXFLK(I), TYFLK(I), TZFLK(I) = direction cosines of the Ith stack particle
struct rec_case_3
{
  int ILOFLK;
  float ETOT, WTFLK, XFLK, YFLK, ZFLK, TXFLK, TYFLK, TZFLK;
};

class Case3Buffer
{
public:
  rec_case_3* data;
  first_rec_case_3* head_rec;
  Case3Buffer(first_rec_case_3* _rec): head_rec(_rec)
  {
    data = new rec_case_3[head_rec->NPFLKA];
  };
  virtual ~Case3Buffer()
  {
    delete [] data;
  };
  inline size_t get_buffer_memory()
  {
    return head_rec->NPFLKA*sizeof(rec_case_3);
  }
};

class SimpleFlukaTrack
{
public:
  vect_3d* Segmets_ends;
  float* energy_deposition_events;
  float total_curved_path;
  int number_of_segments; // the actual number of segments is number_of_segments-1, Segments_ends[0] - starting point of the track
  int number_of_energy_deposition_events;
  SimpleFlukaTrack(int NTRACK, int MTRACK) :
    number_of_segments(NTRACK+1), number_of_energy_deposition_events(MTRACK)
  {
    Segmets_ends = new vect_3d[number_of_segments];
    energy_deposition_events = new float[number_of_energy_deposition_events];
    total_curved_path = 0.;
  };
  virtual ~SimpleFlukaTrack()
  {
    delete [] Segmets_ends;
    delete [] energy_deposition_events;
  };

  inline size_t get_segmets_memory()
  {
    return (number_of_segments)*sizeof(vect_3d);
  }
  inline size_t get_enegry_deposition_memory()
  {
    return number_of_energy_deposition_events*sizeof(float);
  }
  inline size_t get_total_curved_path_memory()
  {
    return sizeof(float);
  }

  float get_average_dEdx()
  {
    float total_energy_deposition(0.0);
    for ( int kv=0; kv<number_of_energy_deposition_events; ++kv)
      {
	total_energy_deposition += energy_deposition_events[kv];
      }
    return total_energy_deposition/total_curved_path;
  }

};


struct Hit
{
  float x,y,z,Etot;
  Hit(const vect_3d* pos, float etot)
  {
    x=pos->x;
    y=pos->y;
    z=pos->z;
    Etot=etot;
  };
  Hit(float _x, float _y, float _z, float etot)
  {
    x=_x;
    y=_y;
    z=_z;
    Etot=etot;
  };
};


class FlukaPartTrack 
{
public:
  std::vector<Hit> hits;
  // Edep[i] is the energy dep. between hits[i] and hits[i+1]
  std::vector<float> Edep;
  // Curved segment path
  // segment_path[i] is the path length between hits[i] and hits[i+1]
  std::vector<float> segment_path;
  int ParticleID_JTRACK;
  int EndTrackICODE;

  inline vect_3d get_last_point()
  {
    auto n = hits.size();
    auto last = hits[n-1];
    return {last.x,last.y,last.z};
  }

  FlukaPartTrack(int ilofkl, float x0, float y0, float z0, float etot)
  {
    hits.push_back({x0,y0,z0,etot});
    ParticleID_JTRACK=ilofkl;
  }

  bool PushBackSimpleFlukaTrack(SimpleFlukaTrack* strk, int JTRACK, float Etrack);

  void print();
};

#endif