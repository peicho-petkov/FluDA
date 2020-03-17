#include "FlukaDumpReader.hh"
#include "fluka_misc.h"
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TGraph.h>
#include "FlukaDumpAnalyser.hh"

class test_an : public FlukaDumpAnalyser
{
public:
    virtual void Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data);
    virtual void Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle);
    virtual void Analyse_Primary_Record(first_rec_case_3 &rec, Case3Buffer *c3b);

    test_an();

    TH1F *tot_dep_E;
    TH1F *sec_hi_dep_E;
    TH1F *pri_hi_dep_E;
    TH1F *pri_p_dep_E;
    TH1F *electr_dep_E;
    TH1F *prot_dep_E;
    TH1F *alpha_dep_E;
    TH1F *He3_dep_E;
    TH1F *H3_dep_E;
    TH1F *H2_dep_E;

    TH2F *tot_dep_E_2d;
    TH2F *sec_hi_dep_E_2d;
    TH2F *pri_hi_dep_E_2d;
    TH2F *pri_p_dep_E_2d;
    TH2F *electr_dep_E_2d;
    TH2F *prot_dep_E_2d;
    TH2F *alpha_dep_E_2d;
    TH2F *He3_dep_E_2d;
    TH2F *H3_dep_E_2d;
    TH2F *H2_dep_E_2d;

    TH1F *pids;
    TH2F *energy_z_pid;
};

test_an::test_an()
{
    tot_dep_E = new TH1F("tot_dep_E", "total energy dep.", 1000, 0., 100.);
    sec_hi_dep_E = new TH1F("sec_hi_dep_E", "secondary HI energy dep.", 1000, 0., 100.);
    pri_hi_dep_E = new TH1F("pri_hi_dep_E", "primary HI energy dep.", 1000, 0., 100.);
    pri_p_dep_E = new TH1F("pri_p_dep_E", "primary p energy dep.", 1000, 0., 100.);
    electr_dep_E = new TH1F("el_dep_E", "electrons energy dep.", 1000, 0., 100.);
    prot_dep_E = new TH1F("p_dep_E", "protons energy dep.", 1000, 0., 100.);
    alpha_dep_E = new TH1F("alpha_dep_E", "alpha energy dep.", 1000, 0., 100.);
    He3_dep_E = new TH1F("He3_dep_E", "He3 energy dep.", 1000, 0., 100.);
    H3_dep_E = new TH1F("H3_dep_E", "H3 energy dep.", 1000, 0., 100.);
    H2_dep_E = new TH1F("H2_dep_E", "H2 energy dep.", 1000, 0., 100.);

    tot_dep_E_2d = new TH2F("tot_dep_E_2d", "total energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    sec_hi_dep_E_2d = new TH2F("sec_hi_dep_E_2d", "secondary HI energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    pri_hi_dep_E_2d = new TH2F("pri_hi_dep_E_2d", "primary HI energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    pri_p_dep_E_2d = new TH2F("pri_p_dep_E_2d", "primary p energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    electr_dep_E_2d = new TH2F("el_dep_E_2d", "electrons energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    prot_dep_E_2d = new TH2F("p_dep_E_2d", "protons energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    alpha_dep_E_2d = new TH2F("alpha_dep_E_2d", "alpha energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    He3_dep_E_2d = new TH2F("He3_dep_E_2d", "He3 energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    H3_dep_E_2d = new TH2F("H3_dep_E_2d", "H3 energy dep.", 1000, 0., 100., 101, -5.05, 5.05);
    H2_dep_E_2d = new TH2F("H2_dep_E_2d", "H2 energy dep.", 1000, 0., 100., 101, -5.05, 5.05);

    pids = new TH1F("pids", "particle ids", 512, -256, 256);
    energy_z_pid = new TH2F("e_z_pid", "energy vs. z and pid", 512, -256, 256, 100, 0., 100.);
}

void test_an::Analyse_Energy_Record(first_rec_case_2 &rec, rec_case_2 &rec_data)
{
    auto prtrks = get_PrimaryParticlesContainer();
    auto n = prtrks.size();
    auto current_beam_track = get_active_beam_track();

    //std::cout << rec.JTRACK<<" rull "<<rec_data.RULL << std::endl;

    pids->Fill(rec.JTRACK);
    tot_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
    tot_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);

    energy_z_pid->Fill(rec.JTRACK, rec_data.ZSCO, rec_data.RULL);

    if (rec.JTRACK == ELECTRON)
    {
        electr_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        electr_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }
    if (rec.JTRACK == PROTON)
    {
        prot_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        prot_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }
    if (rec.JTRACK == HELIUM_4)
    {
        alpha_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        alpha_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }
    if (rec.JTRACK == HELIUM_3)
    {
        He3_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        He3_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }
    if (rec.JTRACK == TRITON)
    {
        H3_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        H3_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }
    if (rec.JTRACK == DEUTERON)
    {
        H2_dep_E->Fill(rec_data.ZSCO, rec_data.RULL);
        H2_dep_E_2d->Fill(rec_data.ZSCO, rec_data.XSCO, rec_data.RULL);
    }

}
void test_an::Analyse_Track_Record(first_rec_case_1 &rec, SimpleFlukaTrack *trk, bool is_beam_particle)
{
    // if (is_beam_particle)
    //     std::cout << " JTRACK " << rec.JTRACK << " av_dEdX " << trk->get_average_dEdx() << " z " << trk->Segmets_ends[0].z << " E " << rec.ETRACK << std::endl;

    if(trk->number_of_energy_deposition_events<1)
    {
        return;    
    };

    if (!is_beam_particle && rec.JTRACK == HEAVYION)
    {
        sec_hi_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        sec_hi_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x,
                              trk->energy_deposition_events[0]);
    }
    else if (is_beam_particle)
    {
        if (rec.JTRACK == HEAVYION)
        {
            pri_hi_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
            pri_hi_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x,
                                  trk->energy_deposition_events[0]);
        }
        else if (rec.JTRACK == PROTON)
        {
            pri_p_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
            pri_p_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x,
                                 trk->energy_deposition_events[0]);
        }
    }

    pids->Fill(rec.JTRACK);

    //std::cout << rec.JTRACK<<" n dep ev. "<<trk->number_of_energy_deposition_events<<" "<<trk->energy_deposition_events[0] << std::endl;

    tot_dep_E->Fill(trk->Segmets_ends[1].z, (double)trk->energy_deposition_events[0]);
    tot_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x,
                       (double)trk->energy_deposition_events[0]);

    energy_z_pid->Fill(rec.JTRACK, trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);

    if (rec.JTRACK == ELECTRON)
    {
        electr_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        electr_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x,
                              trk->energy_deposition_events[0]);
    }

    if (rec.JTRACK == PROTON && !is_beam_particle)
    {
        prot_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        prot_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x, trk->energy_deposition_events[0]);
    }
    if (rec.JTRACK == HELIUM_4)
    {
        alpha_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        alpha_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x, trk->energy_deposition_events[0]);
    }
    if (rec.JTRACK == HELIUM_3)
    {
        He3_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        He3_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x, trk->energy_deposition_events[0]);
    }
    if (rec.JTRACK == TRITON)
    {
        H3_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        H3_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x, trk->energy_deposition_events[0]);
    }
    if (rec.JTRACK == DEUTERON)
    {
        H2_dep_E->Fill(trk->Segmets_ends[1].z, trk->energy_deposition_events[0]);
        H2_dep_E_2d->Fill(trk->Segmets_ends[1].z, trk->Segmets_ends[1].x, trk->energy_deposition_events[0]);
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
    test_an an;
    int nprimary = flr.Analyse(&an);
    TFile rfile(argv[2], "RECREATE");
    std::cout << "Primary particles: " << nprimary << std::endl;

    auto nbins = 10000;
    auto Ebeam_hist = new TH1F("Ebeam", "Ebeam", nbins, 0., 100.);
    auto Ebeam_hist_dz = 100. / nbins;
    auto numEbeam_hist = new TH1F("numEbeam", "numEbeam", nbins, 0., 100.);

    auto dEdx_hist = new TH1F("dEdx", "dEdx", nbins, 0., 100.);

    auto prtrks = an.get_PrimaryParticlesContainer();
    for (auto iptrk = prtrks.begin(); iptrk != prtrks.end(); iptrk++)
    {
        auto trk = *iptrk;
        auto nhits = trk->hits.size();
        for (auto n = 0; n < nhits - 1; ++n)
        {
            for (auto z = trk->hits[n].z; z < trk->hits[n + 1].z; z += Ebeam_hist_dz)
            {
                Ebeam_hist->Fill(z, trk->hits[n].Etot);
                numEbeam_hist->Fill(z);
                dEdx_hist->Fill(z, trk->Edep[n] / trk->segment_path[n] * 1.e2);
            }
        }
    }
    Ebeam_hist->Divide(numEbeam_hist);
    dEdx_hist->Divide(numEbeam_hist);

    auto LET = new TGraph();
    LET->SetName("LET");

    for (auto bin = 3; bin < Ebeam_hist->GetNbinsX() - 3; ++bin)
    {
        auto E0 = Ebeam_hist->GetBinContent(bin - 2);
        auto E1 = Ebeam_hist->GetBinContent(bin - 1);
        auto E2 = Ebeam_hist->GetBinContent(bin);
        auto z2 = Ebeam_hist->GetBinCenter(bin);
        auto E3 = Ebeam_hist->GetBinContent(bin + 1);
        auto E4 = Ebeam_hist->GetBinContent(bin + 2);
        if (numEbeam_hist->GetBinContent(bin - 2) < 1 || numEbeam_hist->GetBinContent(bin - 1) < 1 || numEbeam_hist->GetBinContent(bin + 1) < 1 || numEbeam_hist->GetBinContent(bin + 2) < 1)
        {
            continue;
        }
        //std::cout << z2 << " " << E0 << " " << E2 << " " << E3 << std::endl;
        auto diff = (-E0 + 8. * E1 - 8. * E3 + E4) / (12. * Ebeam_hist_dz); //negative
        LET->SetPoint(LET->GetN(), z2, diff * 1e6 / 1e4);
        // LET->SetPoint(LET->GetN(),(z2+z1)/2.,-(E2-E1)/(z2-z1)*1e6/1e4);
    }

    an.tot_dep_E->Scale(1. / nprimary);
    an.sec_hi_dep_E->Scale(1. / nprimary);
    an.pri_hi_dep_E->Scale(1. / nprimary);
    an.pri_p_dep_E->Scale(1. / nprimary);
    an.electr_dep_E->Scale(1. / nprimary);
    an.prot_dep_E->Scale(1. / nprimary);
    an.alpha_dep_E->Scale(1. / nprimary);
    an.He3_dep_E->Scale(1. / nprimary);
    an.H3_dep_E->Scale(1. / nprimary);
    an.H2_dep_E->Scale(1. / nprimary);

    an.tot_dep_E_2d->Scale(1. / nprimary);
    an.sec_hi_dep_E_2d->Scale(1. / nprimary);
    an.pri_hi_dep_E_2d->Scale(1. / nprimary);
    an.pri_p_dep_E_2d->Scale(1. / nprimary);
    an.electr_dep_E_2d->Scale(1. / nprimary);
    an.prot_dep_E_2d->Scale(1. / nprimary);
    an.alpha_dep_E_2d->Scale(1. / nprimary);
    an.He3_dep_E_2d->Scale(1. / nprimary);
    an.H3_dep_E_2d->Scale(1. / nprimary);
    an.H2_dep_E_2d->Scale(1. / nprimary);

    an.pids->Scale(1. / nprimary);
    an.energy_z_pid->Scale(1. / nprimary);

    an.tot_dep_E->Write();
    an.sec_hi_dep_E->Write();
    an.pri_hi_dep_E->Write();
    an.pri_p_dep_E->Write();
    an.electr_dep_E->Write();
    an.prot_dep_E->Write();
    an.alpha_dep_E->Write();
    an.He3_dep_E->Write();
    an.H3_dep_E->Write();
    an.H2_dep_E->Write();

    an.tot_dep_E_2d->Write();
    an.sec_hi_dep_E_2d->Write();
    an.pri_hi_dep_E_2d->Write();
    an.pri_p_dep_E_2d->Write();
    an.electr_dep_E_2d->Write();
    an.prot_dep_E_2d->Write();
    an.alpha_dep_E_2d->Write();
    an.He3_dep_E_2d->Write();
    an.H3_dep_E_2d->Write();
    an.H2_dep_E_2d->Write();

    an.pids->Write();
    an.energy_z_pid->Write();
    Ebeam_hist->Write();
    numEbeam_hist->Write();
    LET->Write();
    dEdx_hist->Write();
    rfile.Close();
    return 0;
}
