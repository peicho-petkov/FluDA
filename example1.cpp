#include "FlukaDumpReader.hh"
#include "fluka_misc.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TGraph.h>
#include "FlukaDumpAnalyser.hh"
#include "dEdx_Calculator.hh"

class test_an1 : public FlukaDumpAnalyser
{
public:
    virtual void Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data);
    virtual void Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle);
    virtual void Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b);

    test_an1();
    virtual ~test_an1();
    
    dEdx_Calculator* p_calc;
    
};

test_an1::test_an1()
{
    p_calc = new dEdx_Calculator("protons", 100, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
}

test_an1::~test_an1()
{
    delete p_calc;
}

void test_an::Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data)
{
}

void test_an::Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle)
{
    if (rec.JTRACK == PROTON)
    {
        p_cacl->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }
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
    an.p_calc->get_3d_Dose("Proton_Dose")->Write();
    an.p_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Proton_Dose")->Write();
    an.p_calc->get_2d_Dose_profile("xy",0.1,"Dose_xy_at_0_1")->Write();
    an.p_calc->get_2d_Dose_profile("xy",0.1,"Dose_xy_at_0_1")->Write();
    an.p_calc->get_2d_Dose_profile("xy",10.,"Dose_xy_at_10")->Write();
    an.p_calc->get_2d_Dose_profile("xy",21.9,"Dose_xy_at_21_9")->Write();
    an.p_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Proton_Dose_x_y0_z21_9")->Write();
    an.p_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Proton_Dose_x_y0_z10")->Write();
    an.p_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Proton_av_dEdx")->Write();
    an.p_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Proton_dEdx_D")->Write();
    rfile.Close();            
    return 0;
}

