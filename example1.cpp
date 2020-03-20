#include "FlukaDumpReader.hh"
#include "fluka_misc.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TGraph.h>
#include "FlukaDumpAnalyser.hh"

class test_an1 : public FlukaDumpAnalyser
{
public:
    virtual void Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data);
    virtual void Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle);
    virtual void Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b);

    test_an1();
    virtual ~test_an1();
    
    
};

test_an1::test_an1()
{

}

test_an1::~test_an1()
{

}

void test_an::Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data)
{
}

void test_an::Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle)
{
}

void test_an::Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b)
{
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage " << argv[0] << " <inputfile> <outputfile.root>" << std::endl;
        return 1;
    }
    FlukaDumpReader flr(argv[1]);
    std::string outfilename(argv[1]);
    outfilename += ".root";
    test_an1 an;
    int nprimary = flr.Analyse(&an);
    TFile rfile(argv[2], "RECREATE");
    std::cout << "Primary particles: " << nprimary << std::endl;
}
