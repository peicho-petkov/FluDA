#ifndef FLUKADUMPANALYSER_HH
#define FLUKADUMPANALYSER_HH

#include <vector>
#include "coll_tape_struct.h"

class FlukaDumpAnalyser
{
private:
  std::vector<FlukaPartTrack*> PrimaryParticlesContainer;
  FlukaPartTrack* active_beam_track;
  bool collect_beam_tracks;
public:
  virtual void Analyse_Energy_Record(first_rec_case_2& rec, rec_case_2& rec_data)=0;
  virtual void Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack* trk, bool is_beam_particle)=0;
  virtual void Analyse_Primary_Record(first_rec_case_3& rec, Case3Buffer *c3b)=0;
  void set_collection_beam_tracks(bool to_collect) {collect_beam_tracks=to_collect;};
  bool get_collection_beam_tracks() {return collect_beam_tracks;};

  void set_active_beam_track(FlukaPartTrack* prtrk)
  {
    active_beam_track = prtrk;
  }

  FlukaPartTrack* get_active_beam_track()
  {
    return active_beam_track;
  }

  std::vector<FlukaPartTrack*> get_PrimaryParticlesContainer() const
  {
    return PrimaryParticlesContainer;
  }

  void push_back_beam_track(FlukaPartTrack* ptrk)
  {
    PrimaryParticlesContainer.push_back(ptrk);
    active_beam_track=nullptr;
  }

  FlukaDumpAnalyser() : collect_beam_tracks(false), active_beam_track(nullptr) 
  {
  }
  virtual ~FlukaDumpAnalyser()
  {
    for(auto ii=PrimaryParticlesContainer.begin(); ii!=PrimaryParticlesContainer.end(); ++ii)
      {
	delete *ii;
      }
    PrimaryParticlesContainer.clear();
  }
};

#endif
