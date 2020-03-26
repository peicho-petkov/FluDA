#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TApplication.h>
#include <TStyle.h>

#include <string>
#include <vector>
#include <iostream>



int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cerr << "Usage " << argv[0] << " <inputfile.root>" << std::endl;
        return 1;
    }

    TApplication *app = new TApplication("test",0,0);

    TFile rfile(argv[1]);

    auto klist = rfile.GetListOfKeys();

    klist->Print();

    std::vector<TH1F*> dedx_d_histos;

    std::string histo_names[] = {"pri_p_dep_E_1",
                        "sec_hi_dep_E_1",
                        "p_dep_E_1", "alpha_dep_E_1", "He3_dep_E_1","H3_dep_E_1", "H2_dep_E_1",
                        "el_dep_E_1"};
    std::string histo_labels[] = {"All Particles",
                        "Electrons",
                        "#alpha part.", "Heavy Ions", "^{3}He","^{3}H", "^{2}He",
                        "Protons"};

    for ( int ii=0; ii<klist->GetSize(); ++ii)
    {
        std::string kstr(klist->At(ii)->GetName());
       
        if (kstr.find("_dEdx_D_proj")!=std::string::npos)
        {
            std::cout<<kstr<<std::endl;
            dedx_d_histos.push_back((TH1F*)rfile.Get(kstr.c_str()));
        }
    }

    float maxy=0;
    for(auto hist : dedx_d_histos)
    {
        hist->SetAxisRange(0,30);
        auto hmax = hist->GetMaximum();
        if(maxy<hmax)
        {
            maxy = hmax;
        }
    }

    TCanvas c2("total LET");
    c2.cd();
    auto ledall = new TH1F(*dedx_d_histos[0]);
    ledall->GetXaxis()->SetTitle("z [cm]");    
    ledall->GetYaxis()->SetTitle("LET_{D} [keV/#mum]");
    ledall->SetLineColor(1);
    ledall->Draw("hhist");
    

    for(auto hist : dedx_d_histos)
    {
        hist->SetMaximum(maxy*1.2);
    }

    gStyle->SetOptStat(0);

    TCanvas c1("LET");
    c1.cd();
    auto legend = new TLegend(0.15,0.6,0.4,0.89);
    legend->SetFillColorAlpha(0,.8);
    legend->SetLineColorAlpha(0,0.9);

    dedx_d_histos[1]->GetXaxis()->SetTitle("z [cm]");    
    dedx_d_histos[1]->GetYaxis()->SetTitle("LET_{D} [keV/#mum]");

    dedx_d_histos[1]->Draw("hhist");
    dedx_d_histos[1]->SetTitle("");
    dedx_d_histos[1]->SetLineColor(2);
    legend->AddEntry((dedx_d_histos[1]),histo_labels[1].c_str(),"lf");
    for (int ii=2; ii<dedx_d_histos.size();++ii)
    {
        dedx_d_histos[ii]->Draw("hhistsame");
        dedx_d_histos[ii]->SetLineColor(ii+1);
         legend->AddEntry((dedx_d_histos[ii]),histo_labels[ii].c_str(),"lf");
        //dedx_d_histos[ii]->SetFillColorAlpha(ii+1,0.15);
    }
    
    dedx_d_histos[0]->Draw("hhistsame");
    dedx_d_histos[0]->SetLineColor(1);
    dedx_d_histos[0]->SetLineWidth(2);
    legend->AddEntry((dedx_d_histos[0]),histo_labels[0].c_str(),"lf");

    legend->Draw();

    // for(auto hist : dedx_d_histos)
    // {
    //     hist->Draw();
    //     std::string hname(hist->GetName());
    //     hist->SetAxisRange(1,22);
    //     hname+=".png";
    //     gPad->SaveAs(hname.c_str());
    // }
    app->Run();
    while(1) gSystem->ProcessEvents();
    rfile.Close();
    return 0;
}
