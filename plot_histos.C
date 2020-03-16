#include <TFile.h>
#include <TH1.h>
#include <TStyle.h>

void plot_histos()
{
    TFile *rf = new TFile("pE0.18001_d_lenout.root");
    TH1F* tot_dep_E = (TH1F*)rf->Get("tot_dep_E");
    TH1F* sec_hi_dep_E = (TH1F*)rf->Get("sec_hi_dep_E");
    rf->Get("pri_hi_dep_E");
    TH1F* pri_p_dep_E = (TH1F*)rf->Get("pri_p_dep_E");
    rf->Get("el_dep_E");
    TH1F* p_dep_E = (TH1F*)rf->Get("p_dep_E");
    TH1F* alpha_dep_E = (TH1F*)rf->Get("alpha_dep_E");

//    gStyle->SetFillStyle(3001);

    gStyle->SetTitleFont(42);

    tot_dep_E->GetXaxis()->SetTitle("z [cm]");
    tot_dep_E->GetYaxis()->SetTitle("Relative Dose [%]");

    pri_p_dep_E->Scale(1./tot_dep_E->GetMaximum()*100.);
    p_dep_E->Scale(1./tot_dep_E->GetMaximum()*100.);
    alpha_dep_E->Scale(1./tot_dep_E->GetMaximum()*100.);
    sec_hi_dep_E->Scale(1./tot_dep_E->GetMaximum()*100.);
    tot_dep_E->Scale(1./tot_dep_E->GetMaximum()*100.);
    

    tot_dep_E->GetXaxis()->SetRangeUser(0,30);
    tot_dep_E->SetLineColor(kBlue);
    tot_dep_E->SetFillColor(kBlue);
    tot_dep_E->Draw("H");

    pri_p_dep_E->GetXaxis()->SetRangeUser(0,30);
    pri_p_dep_E->SetLineColor(kRed);
    pri_p_dep_E->SetFillColor(kRed);
    pri_p_dep_E->SetFillStyle(3001);
    pri_p_dep_E->Draw("Hsame");
    
    p_dep_E->GetXaxis()->SetRangeUser(0,30);
    p_dep_E->SetLineColor(kGreen);
    p_dep_E->SetFillColor(kGreen);
    p_dep_E->Draw("Hsame");
    
    alpha_dep_E->GetXaxis()->SetRangeUser(0,30);
    alpha_dep_E->SetLineColor(kOrange);
    alpha_dep_E->SetFillColor(kOrange);
    alpha_dep_E->Draw("Hsame");
    
    sec_hi_dep_E->GetXaxis()->SetRangeUser(0,30);
    sec_hi_dep_E->SetLineColor(kViolet);
    sec_hi_dep_E->SetFillColor(kViolet);
    sec_hi_dep_E->Draw("Hsame");
}