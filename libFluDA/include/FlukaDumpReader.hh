#ifndef FLUKADUMPREADER_HH
#define FLUKADUMPREADER_HH

#include "FlukaDumpAnalyser.hh"
#include <fstream>

class FlukaDumpReader
{
private:
  std::string dumpfilename;
  std::ifstream *dumpfile;

public:
  FlukaDumpReader(const char *filename) : dumpfilename(filename)
  {
    dumpfile = new std::ifstream(dumpfilename,
				 std::ios::in | std::ios::binary);
  };
  virtual ~FlukaDumpReader()
  {
    if (dumpfile->is_open())
      {
	dumpfile->close();
	delete dumpfile;
      }
  };
  bool continue_reading() { return !dumpfile->eof();};
  int get_first_int(int*);
  void read_first_rec(int,first_rec_case_1*);
  void read_first_rec(first_rec_case_2*);
  void read_first_rec(int,first_rec_case_3*);
  SimpleFlukaTrack* get_SimpleFlukaTrack(first_rec_case_1*);
  Case3Buffer* get_Case3Buffer(first_rec_case_3*);
  void get_Case2Buffer(first_rec_case_2*, rec_case_2* rec);
  int Analyse(FlukaDumpAnalyser* fda, int events_to_analyse=-1);
};

#endif