#include "FlukaDumpReader.hh"
#include "fluka_misc.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TGraph.h>
#include "FlukaDumpAnalyser.hh"
#include "dEdx_Calculator.hh"

class analyzer : public FlukaDumpAnalyser
{
public:
    virtual void Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data);
    virtual void Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle);
    virtual void Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b);

    analyzer ();
    virtual ~analyzer();
    
    dEdx_Calculator* e_calc;
    
};

analyzer::analyzer()
{
    e_calc = new dEdx_Calculator("electrons", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
}

analyzer::~analyzer()
{
    delete e_calc;
}

void analyzer::Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data)
{
}

void analyzer::Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle)
{
    if (rec.JTRACK == ELECTRON)
    {
        e_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }
}
void analyzer::Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b)
{
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage " << argv[0] << " <inputfile> <outputfile.root>" << std::endl;
        return 1;
    }
     FlukaDumpReader fluread(argv[1]);
    std::string outfilename(argv[1]);
    outfilename += ".root";
    analyzer an;
    int nprimary = fluread.Analyse(&an);
    TFile rfile(argv[2], "RECREATE");
    std::cout << "Primary particles: " << nprimary << std::endl;
    an.e_calc->get_3d_Dose("Electron_Dose")->Write();
    an.e_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Electron_Dose")->Write();
    an.e_calc->get_2d_Dose_profile("xy",0.1,"Dose_xy_at_0_1")->Write();
    an.e_calc->get_2d_Dose_profile("xy",0.1,"Dose_xy_at_0_1")->Write();
    an.e_calc->get_2d_Dose_profile("xy",10.,"Dose_xy_at_10")->Write();
    an.e_calc->get_2d_Dose_profile("xy",21.9,"Dose_xy_at_21_9")->Write();
    an.e_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Electron_Dose_x_y0_z21_9")->Write();
    an.e_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Electron_Dose_x_y0_z10")->Write();
    an.e_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Electron_av_dEdx")->Write();
    an.e_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Electron_dEdx_D")->Write();
    rfile.Close();            
    return 0;
}
