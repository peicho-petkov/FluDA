#include "coll_tape_struct.h"

void FlukaPartTrack::print()
{
  if (hits.size() == 0) return;
  std::cout<<" pIDfl "<<ParticleID_JTRACK<< " x "<<hits[0].x<<" y "<<hits[0].y<<" z "<<hits[0].z<<" Etot "<<hits[0].Etot<<" Edep 0."<<std::endl;
	
  for (int vk=1; vk<hits.size();++vk)
    {
      std::cout<<" pIDfl "<<ParticleID_JTRACK<< " x "<<hits[vk].x<<" y "<<hits[vk].y<<" z "<<hits[vk].z<<" Etot "<<hits[vk].Etot<<" Edep "<<Edep[vk-1]<<std::endl;
    }
}

bool FlukaPartTrack::PushBackSimpleFlukaTrack(SimpleFlukaTrack* strk,int JTRACK, float Etrack)
{
  if (strk->number_of_segments < 1) 
    {
      return false;
    }
  if (ParticleID_JTRACK>=-6 && ParticleID_JTRACK!=JTRACK)
    {
      return false;
    }

  if ( hits.size()==1 && ParticleID_JTRACK<-6)
    if (hits[0].x == strk->Segmets_ends[0].x &&
	hits[0].y == strk->Segmets_ends[0].y &&
	hits[0].z == strk->Segmets_ends[0].z)
      ParticleID_JTRACK=JTRACK;


  int nEdep = Edep.size();

  if (strk->number_of_segments - 1 == strk->number_of_energy_deposition_events)
    for (int vv = 0; vv < strk->number_of_segments - 1; ++vv)
      {
	int last_hit_index = hits.size() - 1;
	if (hits[last_hit_index].x == strk->Segmets_ends[vv].x &&
	    hits[last_hit_index].y == strk->Segmets_ends[vv].y &&
	    hits[last_hit_index].z == strk->Segmets_ends[vv].z) // check if the begining of the segment is the end of the prev. segment
	  {
	    hits.push_back({&(strk->Segmets_ends[vv + 1]), Etrack});
	    Edep.push_back(strk->energy_deposition_events[vv]);
	    segment_path.push_back(strk->total_curved_path/(strk->number_of_segments - 1));
	  }
	else
	  {
	    return false;
	  }
			
      }
  else // very unlikely case, but added for completeness
    {
      for (int kk = 0; kk < (strk->number_of_segments - 1 - strk->number_of_energy_deposition_events); ++kk)
	{
	  //add zeros
	  Edep.push_back(0.);
	}
      for (int kk = 0; kk < (strk->number_of_energy_deposition_events); ++kk)
	{
	  //add zeros
	  Edep.push_back(strk->energy_deposition_events[kk]);
	}
    }
  return true;
};
