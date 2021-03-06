#include "dEdx_Calculator.hh"
#include <iostream>
#include <cstdlib>

dEdx_Calculator::dEdx_Calculator(const char *name, int nbinsx, double xlow, double xup,
                                 int nbinsy, double ylow, double yup,
                                 int nbinsz, double zlow, double zup) : m_name(name)
{
    m_dedx_Edep_accumulator = new TH3D(m_dedx_edep_name, m_dedx_edep_name,
                                       nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
    m_dedx_Edep_accumulator->AddDirectory(kFALSE);
    
    m_dedx_accumulator =  new TH3D(m_dedx_name, m_dedx_name,
                                       nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
    m_dedx_accumulator->AddDirectory(kFALSE);                               
    
    m_Edep_accumulator =  new TH3D(m_edep_name, m_edep_name,
                                       nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
    m_Edep_accumulator->AddDirectory(kFALSE);

    m_nentries = new TH3I(m_nentries_name, m_nentries_name,
                          nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
    m_nentries->AddDirectory(kFALSE);

    set_density();
}

void dEdx_Calculator::SetAxisRange(float xmin, float xmax, Option_t *axis)
{
    m_Edep_accumulator->SetAxisRange(xmin, xmax, axis);
    m_dedx_accumulator->SetAxisRange(xmin, xmax, axis);
    m_dedx_Edep_accumulator->SetAxisRange(xmin, xmax, axis);
    m_nentries->SetAxisRange(xmin, xmax, axis);
};

TH3D* dEdx_Calculator::get_3d_av_dEdx(const char* histoname)
{
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx";
        hname += "_xyz";
    }
    TH3D* dedx = (TH3D*)m_dedx_accumulator->Clone(hname.c_str());
    TH3D* nentries = (TH3D*)m_nentries->Clone("tmp_nentries");
    dedx->Divide(nentries);
    dedx->Scale(100.); // units [keV/um]
    delete nentries;
    return dedx;
}

TH3D* dEdx_Calculator::get_3d_Dose(const char* histoname)
{
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_Edep";
        hname += "_xyz";
    }
    TH3D* edep = (TH3D*)m_Edep_accumulator->Clone(hname.c_str());
    float dx = edep->GetXaxis()->GetBinWidth(-1);
    float dy = edep->GetYaxis()->GetBinWidth(-1);
    float dz = edep->GetZaxis()->GetBinWidth(-1);
    float dV = dx*dy*dz;
    edep->Scale(1./(m_density*dV));
    return edep;
}

TH3D* dEdx_Calculator::get_3d_dEdx_D(const char* histoname)
{
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx_D";
        hname += "_xyz";
    }
    TH3D* dE = (TH3D*)m_Edep_accumulator->Clone("tmpEdep_3d");
    TH3D* dEdx_dE = (TH3D*)m_dedx_Edep_accumulator->Clone(hname.c_str());
    dEdx_dE->Divide(dE);
    dEdx_dE->Scale(100.); // units [keV/um]
    delete dE;
    return dEdx_dE;
}

void dEdx_Calculator::ResetAllAxisRange()
{
    m_Edep_accumulator->GetXaxis()->SetRange(0, 0);
    m_dedx_accumulator->GetXaxis()->SetRange(0, 0);
    m_dedx_Edep_accumulator->GetXaxis()->SetRange(0, 0);
    m_nentries->GetXaxis()->SetRange(0, 0);

    m_Edep_accumulator->GetYaxis()->SetRange(0, 0);
    m_dedx_accumulator->GetYaxis()->SetRange(0, 0);
    m_dedx_Edep_accumulator->GetYaxis()->SetRange(0, 0);
    m_nentries->GetYaxis()->SetRange(0, 0);

    m_Edep_accumulator->GetZaxis()->SetRange(0, 0);
    m_dedx_accumulator->GetZaxis()->SetRange(0, 0);
    m_dedx_Edep_accumulator->GetZaxis()->SetRange(0, 0);
    m_nentries->GetZaxis()->SetRange(0, 0);
};

TH2D* dEdx_Calculator::get_2d_Dose_profile(Option_t* plane, float plane_pos,
                                const char* histoname)
{
    TH3D* h3d = get_3d_Dose("tmp3dprof");
    TH2D* prof_2d = get_2d_profile(plane,plane_pos,h3d);
    prof_2d->SetName(histoname);
    delete h3d;
    return prof_2d;
}                                
    
TH2D* dEdx_Calculator::get_2d_dEdx_D_profile(Option_t* plane, float plane_pos,
                                const char* histoname)
{
    TH3D* h3d = get_3d_dEdx_D("tmp3dprof");
    TH2D* prof_2d = get_2d_profile(plane,plane_pos,h3d);
    prof_2d->SetName(histoname);
    delete h3d;
    return prof_2d;
}

TH2D* dEdx_Calculator::get_2d_av_dEdx_profile(Option_t* plane, float plane_pos,
                                const char* histoname)
{
    TH3D* h3d = get_3d_av_dEdx("tmp3dprof");
    TH2D* prof_2d = get_2d_profile(plane,plane_pos,h3d);
    prof_2d->SetName(histoname);
    delete h3d;
    return prof_2d;
}


TH1D* dEdx_Calculator::get_1d_Dose_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname)
{
    TH3D* h3d = get_3d_Dose("tmp3dprof");
    TH1D* prof_1d = get_1d_profile(projaxis,
                                    axcoord1,coord1,
                                    axcoord2,coord2,
                                    h3d);
    prof_1d->SetName(histoname);
    delete h3d;
    return prof_1d;    
}

    
TH1D* dEdx_Calculator::get_1d_dEdx_D_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname)
{
    TH3D* h3d = get_3d_dEdx_D("tmp3dprof");
    TH1D* prof_1d = get_1d_profile(projaxis,
                                    axcoord1,coord1,
                                    axcoord2,coord2,
                                    h3d);
    prof_1d->SetName(histoname);
    delete h3d;
    return prof_1d;    
}

TH1D* dEdx_Calculator::get_1d_av_dEdx_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname)
{
    TH3D* h3d = get_3d_av_dEdx("tmp3dprof");
    TH1D* prof_1d = get_1d_profile(projaxis,
                                    axcoord1,coord1,
                                    axcoord2,coord2,
                                    h3d);
    prof_1d->SetName(histoname);
    delete h3d;
    return prof_1d;    
}

TH2D* dEdx_Calculator::get_2d_av_dEdx_projection(Option_t* prjaxs,const char* histoname)
{
    std::string _optiopns((char *)prjaxs);
    if(_optiopns.size()!=2)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_2d_av_dEdx_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx_";
        hname += (char*)prjaxs;
    }
    TH2D* dedx = (TH2D*)m_dedx_accumulator->Project3D(prjaxs); 
    dedx->SetName(hname.c_str());
    TH2D* nentries = (TH2D*)m_nentries->Project3D(prjaxs); 
    dedx->Divide(nentries);
    dedx->Scale(100.); // units [keV/um]
    delete nentries;
    return dedx;
}

TH1D* dEdx_Calculator::get_1d_av_dEdx_projection(Option_t* prjax,const char* histoname)
{
    std::string _optiopns((char *)prjax);
    if(_optiopns.size()!=1)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_1d_av_dEdx_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx_";
        hname += (char*)prjax;
    }
    TH1D* dedx = (TH1D*)m_dedx_accumulator->Project3D(prjax); 
    dedx->SetName(hname.c_str());
    TH1D* nentries = (TH1D*)m_nentries->Project3D(prjax); 
    dedx->SetName(hname.c_str());
    dedx->Divide(nentries);
    dedx->Scale(100.); // units [keV/um]
    delete nentries;
    return dedx;
}

TH2D* dEdx_Calculator::get_2d_Dose_projection(Option_t* prjaxs,const char* histoname)
{
    std::string _optiopns((char *)prjaxs);
    if(_optiopns.size()!=2)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_2d_Dose_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_Dose_";
        hname += (char*)prjaxs;
    }
    TH3D* edep3d = (TH3D*)m_Edep_accumulator->Clone("tmp_edep3d");
    float dx = edep3d->GetXaxis()->GetBinWidth(-1);
    float dy = edep3d->GetYaxis()->GetBinWidth(-1);
    float dz = edep3d->GetZaxis()->GetBinWidth(-1);
    float dV = dx*dy*dz;
    edep3d->Scale(1./(m_density*dV));
    TH2D* edep = (TH2D*)edep3d->Project3D(prjaxs);
    delete edep3d; 
    edep->SetName(hname.c_str());
    edep->Scale(dx);
    return edep;
}

TH1D* dEdx_Calculator::get_1d_Dose_projection(Option_t* prjax,const char* histoname)
{
    std::string _optiopns((char *)prjax);
    if(_optiopns.size()!=1)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_1d_Dose_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_Dose_";
        hname += (char*)prjax;
    }
    TH3D* edep3d = (TH3D*)m_Edep_accumulator->Clone("tmp_edep3d");
    float dx = edep3d->GetXaxis()->GetBinWidth(-1);
    float dy = edep3d->GetYaxis()->GetBinWidth(-1);
    float dz = edep3d->GetZaxis()->GetBinWidth(-1);
    float dV = dx*dy*dz;
    edep3d->Scale(1./(m_density*dV));
    TH1D* edep = (TH1D*)edep3d->Project3D(prjax);
    delete edep3d; 
    edep->SetName(hname.c_str());
    edep->Scale(dx*dx);
    return edep;
}

TH2D* dEdx_Calculator::get_2d_dEdx_D_projection(Option_t* prjaxs, const char* histoname)
{
    std::string _optiopns((char *)prjaxs);
    if(_optiopns.size()!=2)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_2d_dEdx_D_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx_D_";
        hname += (char*)prjaxs;
    }
    TH2D* dE = (TH2D*)m_Edep_accumulator->Project3D(prjaxs);
    TH2D* dEdx_dE = (TH2D*)m_dedx_Edep_accumulator->Project3D(prjaxs);
    dEdx_dE->SetName(hname.c_str());
    dEdx_dE->Divide(dE);
    delete dE;
    return dEdx_dE;
}

TH1D* dEdx_Calculator::get_1d_dEdx_D_projection(Option_t* prjax, const char* histoname)
{
    std::string _optiopns((char *)prjax);
    if(_optiopns.size()!=1)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_1d_dEdx_D_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_dEdx_D_";
        hname += (char*)prjax;
    }
    TH1D* dE = (TH1D*)m_Edep_accumulator->Project3D(prjax);

    // TH1I* n = (TH1I*)m_nentries->Project3D(prjax);
    // double sc = (n->GetMaximum()/100.);
    // n->Scale(1/sc);
    // n->Scale(sc);
    // n->Divide(n);
    // std::cout<<"dE->GetMaximum() "<<dE->GetMaximum()<<std::endl;
    // dE->Multiply(n);
    // std::cout<<n->GetMaximum()<<std::endl;
    // std::cout<<"dE->GetMaximum() "<<dE->GetMaximum()<<std::endl;

    TH1D* dEdx_dE = (TH1D*)m_dedx_Edep_accumulator->Project3D(prjax);
    dEdx_dE->SetName(hname.c_str());
    dEdx_dE->Divide(dE);
    dEdx_dE->Scale(100.); // units [keV/um]
    delete dE;
    return dEdx_dE;
}


TH2D* dEdx_Calculator::get_2d_profile(Option_t* plane, float plane_pos,TH3D* h3d) {

    std::string strplane((char*)plane);
    std::string traxes("xyz");

    for (std::string::iterator it = strplane.begin(); it != strplane.end(); it++)
    {
        char ax = *it;
        int pos = traxes.find(ax);
        traxes.erase(pos,1);
    }
    
    h3d->SetAxisRange(plane_pos,plane_pos,traxes.c_str());

    TH2D* prof_2d = (TH2D*)h3d->Project3D(plane);

    h3d->GetXaxis()->SetRange(0,0);
    h3d->GetYaxis()->SetRange(0,0);
    h3d->GetZaxis()->SetRange(0,0);

    return prof_2d;
}

TH1D* dEdx_Calculator::get_1d_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                      TH3D* h3d) {
    
    h3d->SetAxisRange(coord1,coord1,axcoord1);
    h3d->SetAxisRange(coord2,coord2,axcoord2);

    TH1D* prof_1d = (TH1D*)h3d->Project3D(projaxis);

    h3d->GetXaxis()->SetRange(0,0);
    h3d->GetYaxis()->SetRange(0,0);
    h3d->GetZaxis()->SetRange(0,0);

    return prof_1d;
}

TH1I* dEdx_Calculator::get_1d_nentries_projection(Option_t* prjax,const char* histoname)
{
    std::string _optiopns((char *)prjax);
    if(_optiopns.size()!=1)
    {
        std::cerr<<_optiopns<<" is not a valid option for dEdx_Calculator::get_1d_nentries_projection"<<std::endl;
    }
    std::string hname(histoname);
    if (hname == "")
    {
        hname = m_name;
        hname += "_nentries_";
        hname += (char*)prjax;
    }
    TH1I* dedx = (TH1I*)m_nentries->Project3D(prjax); 
    dedx->SetName(hname.c_str());
    return dedx;
}

