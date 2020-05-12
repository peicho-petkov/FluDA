#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

void plot_histos()
{
    TFile *rf = new TFile("hiE0.35001.root");
    
    TH1F *tot_dep_E = (TH1F *)rf->Get("tot_dep_E");
    TH1F *sec_hi_dep_E = (TH1F *)rf->Get("sec_hi_dep_E");
    TH1F *pri_hi_dep_E = (TH1F *)rf->Get("pri_hi_dep_E");
    TH1F *pri_p_dep_E = (TH1F *)rf->Get("pri_p_dep_E");
    TH1F *el_dep_E = (TH1F *)rf->Get("el_dep_E");
    TH1F *p_dep_E = (TH1F *)rf->Get("p_dep_E");
    TH1F *alpha_dep_E = (TH1F *)rf->Get("alpha_dep_E");
    TH1F *He3_dep_E = (TH1F *)rf->Get("He3_dep_E");
    TH1F *H3_dep_E = (TH1F *)rf->Get("H3_dep_E");
    TH1F *H2_dep_E = (TH1F *)rf->Get("H2_dep_E");
    TH1F *dEdx = (TH1F *)rf->Get("dEdx");

    int nhistos = 8;
    TObject* histos[] = {pri_hi_dep_E,
                        sec_hi_dep_E, 
                        el_dep_E,
                        p_dep_E, alpha_dep_E, He3_dep_E,H3_dep_E, H2_dep_E};
    std::string histo_names[] = {"pri_hi_dep_E_1",
                        "sec_hi_dep_E_1",
                        "p_dep_E_1", "alpha_dep_E_1", "He3_dep_E_1","H3_dep_E_1", "H2_dep_E_1",
                        "el_dep_E_1"};
    std::string histo_labels[] = {"Primary Heavy Ions",
                        "Seocndary Heavy Ions",
                        "Secondary Protons", "Secondary #alpha part.", "Secondary ^{3}He","Secondary ^{3}H", "Secondary ^{2}He",
                        "Secondary Electrons"};
    TH1F* sumhist[8];
    //    gStyle->SetFillStyle(3001);
    gStyle->SetOptStat(0);
    float opt = 0.15;
    float upperleft=100.;
    float label_width=20.;


    gStyle->SetTitleFont(42);

    tot_dep_E->GetXaxis()->SetTitle("z [cm]");
    tot_dep_E->GetYaxis()->SetTitle("Relative Dose [%]");
    tot_dep_E->SetTitle("^{12}C 350 MeV/n Relative Dose Profile");

    pri_p_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    pri_hi_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    p_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    alpha_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    He3_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    H3_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    sec_hi_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    el_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);
    tot_dep_E->Scale(1. / tot_dep_E->GetMaximum() * 100.);


    //auto legend = new TLegend(0.1,0.6,0.4,0.9);
    //legend->SetHeader("^{12}C 350 GeV/n","C"); // option "C" allows to center the header
    auto legend = new TLegend(0.15,0.6,0.4,0.89);
    legend->SetFillColorAlpha(0,.8);
    legend->SetLineColorAlpha(0,0.9);

    tot_dep_E->GetXaxis()->SetRangeUser(0, 30);
    tot_dep_E->SetLineColor(1);
    // tot_dep_E->SetFillColorAlpha(0,opt);
    tot_dep_E->Draw("HHIST");
    legend->AddEntry(tot_dep_E,"Total","lf");
    
    if (pri_p_dep_E->GetEntries() > 0)
    {
        pri_p_dep_E->GetXaxis()->SetRangeUser(0, 30);
        pri_p_dep_E->SetLineColor(2);
        pri_p_dep_E->SetFillColorAlpha(2,opt);
        pri_p_dep_E->Draw("HHISTsame");
        legend->AddEntry(pri_p_dep_E,"Primary Protons","lf");
    }
    else
    {
        pri_hi_dep_E->GetXaxis()->SetRangeUser(0, 30);
        pri_hi_dep_E->SetLineColor(2);
        pri_hi_dep_E->SetFillColorAlpha(2,opt);
        pri_hi_dep_E->Draw("HHISTsame");
        legend->AddEntry(pri_hi_dep_E,"Primary Heavy Ions","lf");
    }

    sec_hi_dep_E->GetXaxis()->SetRangeUser(0, 30);
    sec_hi_dep_E->SetLineColor(3);
    sec_hi_dep_E->SetFillColorAlpha(3,opt);
    sec_hi_dep_E->Draw("HHISTsame");
    legend->AddEntry(sec_hi_dep_E,"Secondary Heavy Ions","lf");

    p_dep_E->GetXaxis()->SetRangeUser(0, 30);
    p_dep_E->SetLineColor(4);
    p_dep_E->SetFillColorAlpha(4,opt);
    p_dep_E->Draw("HHISTsame");
    legend->AddEntry(p_dep_E,"Secondary Protons","lf");

    alpha_dep_E->GetXaxis()->SetRangeUser(0, 30);
    alpha_dep_E->SetLineColor(5);
    alpha_dep_E->SetFillColorAlpha(5,opt);
    alpha_dep_E->Draw("HHISTsame");
    legend->AddEntry(alpha_dep_E,"Secondary #alpha part.","lf");
    
    He3_dep_E->GetXaxis()->SetRangeUser(0, 30);
    He3_dep_E->SetLineColor(6);
    He3_dep_E->SetFillColorAlpha(6,opt);
    He3_dep_E->Draw("HHISTsame");
    legend->AddEntry(He3_dep_E,"Secondary ^{3}He","lf");
    

    H3_dep_E->GetXaxis()->SetRangeUser(0, 30);
    H3_dep_E->SetLineColor(7);
    H3_dep_E->SetFillColorAlpha(7,opt);
    H3_dep_E->Draw("HHISTsame");
    legend->AddEntry(H3_dep_E,"Secondary ^{3}H","lf");
    

    H2_dep_E->GetXaxis()->SetRangeUser(0, 30);
    H2_dep_E->SetLineColor(8);
    H2_dep_E->SetFillColorAlpha(8,opt);
    H2_dep_E->Draw("HHISTsame");
    legend->AddEntry(H2_dep_E,"Secondary ^{2}H","lf");
        
    el_dep_E->GetXaxis()->SetRangeUser(0, 30);
    el_dep_E->SetLineColor(9);
    el_dep_E->SetFillColorAlpha(9,opt);
    el_dep_E->Draw("HHISTsame");
    legend->AddEntry(el_dep_E,"Secondary electrons","lf");
    
    legend->Draw();


    TCanvas * kvc = new TCanvas();
    kvc->cd();

    auto legend1 = new TLegend(0.15,0.6,0.4,0.89);
    legend1->SetFillColorAlpha(0,.8);
    legend1->SetLineColorAlpha(0,0.9);

    TH1F* sum = new TH1F((TH1F&)*(histos[nhistos-1]));
    sum->SetName(histo_names[nhistos-1].c_str());
    sum->SetFillColorAlpha(nhistos+1,opt);
    sum->SetLineColor(nhistos+1);
    sum->SetFillStyle(1001);
    sumhist[nhistos-1]=sum;
    // sum->Draw("histo");

    for (int kk=nhistos-2; kk>-1; --kk)
    {
        TH1F* sum1 = new TH1F((TH1F&)*sum);
        sum1->SetName(histo_names[kk].c_str());
        sum1->Add((TH1*)histos[kk]);
        sum1->SetFillColorAlpha(kk+2,opt);
        sum1->SetLineColor(kk+2);
        //sum1->SetFillStyle(1001);
        sumhist[kk]=sum1;
        // sum1->Draw("histosame");
        sum = new TH1F((TH1F&)*(sum1));
    }

    tot_dep_E->Draw("hhisto");
    tot_dep_E->SetLineColor(1);
    //tot_dep_E->SetFillColor(1);
    legend1->AddEntry(tot_dep_E,"Total","lf");

    for (int kk=0; kk<nhistos; ++kk)
    {
        sumhist[kk]->Draw("hhistosame");
        legend1->AddEntry(sumhist[kk],histo_labels[kk].c_str(),"lf");
    }
    legend1->Draw();

    TCanvas * vvc = new TCanvas();
    vvc->cd();
    dEdx->Rebin(10);
    dEdx->Scale(0.1);
    dEdx->Multiply(tot_dep_E);
    dEdx->Scale(1./tot_dep_E->Integral());
    dEdx->Draw("hhisto");
}
