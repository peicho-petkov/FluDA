void unnamed()
{
   TCanvas *c1 = new TCanvas();
   TPaveText *pt = new TPaveText(0.3531805,0.9342405,0.6468195,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("total energy dep.");
   pt->Draw();
   
   pt = new TPaveText(4.669052,82.86715,10.35778,88.72645,"br");
   pt_LaTex = pt->AddText("Primary Protons");
   pt->Draw();
   
   pt = new TPaveText(1.663685,92.49313,3.864043,98.35243,"br");
   pt->SetFillColor(4);
   pt->SetLineColor(4);
   pt->Draw();
   TPave *pave = new TPave(1.663685,82.72764,3.810376,88.58694,4,"br");
   pave->SetFillColor(2);
   pave->SetFillStyle(3001);
   pave->Draw();
   
   pt = new TPaveText(4.722719,92.07461,10.41145,97.93391,"br");
   pt_LaTex = pt->AddText("Total");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->ToggleToolBar();
}
