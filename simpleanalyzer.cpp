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
    dEdx_Calculator* alpha_calc;
    dEdx_Calculator* heavyion_calc;
    dEdx_Calculator* helium3_calc;
    dEdx_Calculator* triton_calc;
    dEdx_Calculator* deutron_calc;
    dEdx_Calculator* p_calc;
    dEdx_Calculator* all;

};

analyzer::analyzer()
{
    e_calc = new dEdx_Calculator("electrons", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    alpha_calc = new dEdx_Calculator("alpha", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    heavyion_calc = new dEdx_Calculator("heavyion", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    helium3_calc = new dEdx_Calculator("helium3", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    triton_calc = new dEdx_Calculator("triton", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    deutron_calc = new dEdx_Calculator("deutron", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    p_calc = new dEdx_Calculator("protons", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
    all = new dEdx_Calculator("all", 101, -5.05, 5.05,100, -5.05, 5.05, 1000,0.,100.);
}

analyzer::~analyzer()
{
    delete e_calc;
    delete alpha_calc;
    delete heavyion_calc;
    delete helium3_calc;
    delete triton_calc;
    delete deutron_calc;
    delete p_calc;    
    delete all;
}

void analyzer::Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data)
{
}

void analyzer::Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle)
{
    if (trk->number_of_energy_deposition_events<1) 
    {
        return;
    }
    
    all->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                          trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    
    if (rec.JTRACK == ELECTRON)
    {
        e_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

     if (rec.JTRACK == HELIUM_4)
    {
        alpha_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

    if (rec.JTRACK == HEAVYION)
    {
        heavyion_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

     if (rec.JTRACK == HELIUM_3)
    {
        helium3_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

       if (rec.JTRACK == TRITON)
    {
        triton_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

     if (rec.JTRACK == DEUTERON)
    {
        deutron_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }

    if (rec.JTRACK == PROTON)
    {
        p_calc->set_dEdx_Edep(trk->Segmets_ends[1].x, trk->Segmets_ends[1].y, trk->Segmets_ends[1].z,
                              trk->energy_deposition_events[0]/trk->total_curved_path ,trk->energy_deposition_events[0]);
    }
}
void analyzer::Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b)
{
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cerr << "Usage " << argv[0] << " <inputfiles> <outputfile.root>" << std::endl;
        return 1;
    }
    analyzer an;
    int nprimary = 0;
    for(int b=1;b<argc -1;b++)
    {
    FlukaDumpReader fluread(argv[b]);
    nprimary += fluread.Analyse(&an);
    }

    TFile rfile(argv[argc -1], "RECREATE");
    std::cout << "Primary particles: " << nprimary << std::endl;

    {// all
    auto hist = an.all->get_3d_Dose("Total_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.all->get_1d_Dose_profile("z","x",0.,"y",0.,"Total_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.all->get_1d_Dose_profile("x","y",0.,"z",21.9,"Total_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.all->get_1d_Dose_profile("x","y",0.,"z",10.,"Total_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.all->get_1d_Dose_projection("z","Total_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.all->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Total_av_dEdx_prof")->Write();
    an.all->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Total_dEdx_D_prof")->Write();
    an.all->get_1d_dEdx_D_projection("z","Total_dEdx_D_proj")->Write();
    }
    
    {// electrons
    auto hist = an.e_calc->get_3d_Dose("Electron_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.e_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Electron_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.e_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Electron_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.e_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Electron_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.e_calc->get_1d_Dose_projection("z","Electron_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.e_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Electron_av_dEdx_prof")->Write();
    an.e_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Electron_dEdx_D_prof")->Write();
    an.e_calc->get_1d_dEdx_D_projection("z","Electron_dEdx_D_proj")->Write();
    }

    {//alpha
    auto hist = an.alpha_calc->get_3d_Dose("Alpha_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.alpha_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Alpha_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.alpha_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Alpha_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.alpha_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Alpha_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.alpha_calc->get_1d_Dose_projection("z","Alpha_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.alpha_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Alpha_av_dEdx_prof")->Write();
    an.alpha_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Alpha_dEdx_D_prof")->Write();
    an.alpha_calc->get_1d_dEdx_D_projection("z","Alpha_dEdx_D_proj")->Write();

    }

    { //heavyion
    auto hist = an.heavyion_calc->get_3d_Dose("Heavyion_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.heavyion_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Heavyion_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.heavyion_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Heavyion_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.heavyion_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Heavyion_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.heavyion_calc->get_1d_Dose_projection("z","Heavyion_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.heavyion_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Heavyion_av_dEdx_prof")->Write();
    an.heavyion_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Heavyion_dEdx_D_prof")->Write();
    an.heavyion_calc->get_1d_dEdx_D_projection("z","Heavyion_dEdx_D_proj")->Write();

    }

    { //helium3
    auto hist = an.helium3_calc->get_3d_Dose("Helium3_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.helium3_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Helium3_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.helium3_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Helium3_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.helium3_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Helium3_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.helium3_calc->get_1d_Dose_projection("z","Helium3_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.helium3_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Helium3_av_dEdx_prof")->Write();
    an.helium3_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Helium3_dEdx_D_prof")->Write();
    an.helium3_calc->get_1d_dEdx_D_projection("z","Helium3_dEdx_D_proj")->Write();

    }

    { //triton
    auto hist = an.triton_calc->get_3d_Dose("Triton_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.triton_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Triton_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.triton_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Triton_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.triton_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Triton_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.triton_calc->get_1d_Dose_projection("z","Triton_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.triton_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Triton_av_dEdx_prof")->Write();
    an.triton_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Triton_dEdx_D_prof")->Write();
    an.triton_calc->get_1d_dEdx_D_projection("z","Triton_dEdx_D_proj")->Write();

    }

    {// deutron
    auto hist = an.deutron_calc->get_3d_Dose("Deutron_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.deutron_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Deutron_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.deutron_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Deutron_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.deutron_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Deutron_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.deutron_calc->get_1d_Dose_projection("z","Deutron_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.deutron_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Deutron_av_dEdx_prof")->Write();
    an.deutron_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Deutron_dEdx_D_prof")->Write();
    an.deutron_calc->get_1d_dEdx_D_projection("z","Deutron_dEdx_D_proj")->Write();

    }

     {// protons
    auto hist = an.p_calc->get_3d_Dose("Proton_Dose");
    hist->Scale(1./nprimary);
    hist->  Write();

    auto hist1 = an.p_calc->get_1d_Dose_profile("z","x",0.,"y",0.,"Proton_Dose_z_prof");
    hist1-> Scale(1./nprimary);
    hist1-> Write();

    auto hist2 = an.p_calc->get_1d_Dose_profile("x","y",0.,"z",21.9,"Proton_Dose_x_y0_z21_9_prof");
    hist2-> Scale(1./nprimary);
    hist2-> Write();

    auto hist3 = an.p_calc->get_1d_Dose_profile("x","y",0.,"z",10.,"Proton_Dose_x_y0_z10_prof");
    hist3-> Scale(1./nprimary);
    hist3-> Write();

    auto hist4 = an.p_calc->get_1d_Dose_projection("z","Proton_Dose_z_proj");
    hist4-> Scale(1./nprimary);
    hist4-> Write();

    an.p_calc->get_1d_av_dEdx_profile("z","x",0.,"y",0.,"Proton_av_dEdx_prof")->Write();
    an.p_calc->get_1d_dEdx_D_profile("z","x",0.,"y",0.,"Proton_dEdx_D_prof")->Write();
    an.p_calc->get_1d_dEdx_D_projection("z","Proton_dEdx_D_proj")->Write();

    }
    
    {//LET
    auto DoseZlet = an.all->get_1d_Dose_projection("z","Total_Dose_LETweighted_z");
    DoseZ-> Scale(1./nprimary);
    DoseZlet->  Write();
        
    auto LETd = an.all->get_1d_dEdx_D_projection("z","Total_dEdx_D_z");
    double k=0.055; // сложи точната стойност
    DoseZlet->Add(LETd,k);
    DoseZlet->Write(); 
       
    }

    rfile.Close();
    return 0;
}
