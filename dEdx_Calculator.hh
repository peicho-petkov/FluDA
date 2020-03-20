#ifndef DEDX_CALCULATOR_HH
#define DEDX_CALCULATOR_HH
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <string>



class dEdx_Calculator
{
    private:
    TH3F *m_dedx_accumulator;
    TH3F *m_dedx_Edep_accumulator;
    TH3F *m_Edep_accumulator;
    TH3F *m_nentries;

    const char* m_dedx_name = "dedx_accumulator";
    const char* m_edep_name = "edep_accumulator";
    const char* m_dedx_edep_name = "dedx_accumulator";
    const char* m_nentries_name = "nentries";
    std::string m_name;

    float m_density;

    public:
    dEdx_Calculator(const char* name, int nbinsx, double xlow, double xup,
                    int nbinsy, double ylow, double yup,
                    int nbinsz, double zlow, double zup);
    
    void set_dEdx_Edep(float x, float y, float z, float dedx, float Edep)
    {
        m_Edep_accumulator->Fill(x,y,z,Edep);
        m_dedx_accumulator->Fill(x,y,z,dedx);
        m_dedx_Edep_accumulator->Fill(x,y,z,dedx*Edep);
        m_nentries->Fill(x,y,z);
    }

    void SetAxisRange(float xmin, float xmax, Option_t *axis);

    void ResetAllAxisRange();

    void set_density(float density = 1.)
    {
        m_density=density;
    }

    float get_density()
    {
        return m_density;
    }

    // 3D distributions of the:
    // - average dEdx
    TH3F* get_3d_av_dEdx(const char* histoname = (const char*)0);
    // - Dose (dE/(density*dV) = dE/dm) - get_3d_Dose
    TH3F* get_3d_Dose(const char* histoname = (const char*)0);
    // - Dose averaged dE/dx - get_3d_dEdx_D
    TH3F* get_3d_dEdx_D(const char* histoname = (const char*)0);

    // 2D profiles of the spatial distributions 
    // plane - two characters string of "x", "y" or "z"
    // plane_pos - coordinate of the plane along the transfersal to the plane axis 

    TH2F* get_2d_Dose_profile(Option_t* plane, float plane_pos,
                                const char* histoname );
    
    TH2F* get_2d_dEdx_D_profile(Option_t* plane, float plane_pos,
                                const char* histoname );

    TH2F* get_2d_av_dEdx_profile(Option_t* plane, float plane_pos,
                                const char* histoname );

    // 1D profiles of the spatial distributions 
    // projaxis - axis to get the profile along, one characters string of "x", "y" or "z"
    // axcoord1 and axcoord2 - axeses perpeditular to each other and both perpendicular ot projaxis
    // coord1 and coord2 - the coordinates of the projaxis along axcoord1 and axcoord2

    TH1F* get_1d_Dose_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname );

   TH1F* get_1d_dEdx_D_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname );

    TH1F* get_1d_av_dEdx_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                const char* histoname );


    // generic profile functions
    TH2F* get_2d_profile(Option_t* plane, float plane_pos,TH3F* h3d);

    TH1F* get_1d_profile(Option_t* projaxis, 
                                      Option_t* axcoord1, float coord1,
                                      Option_t* axcoord2, float coord2,
                                      TH3F* h3d);

    // 1D and 2D projections of the correspondig  3D destributions

    TH1F* get_1d_av_dEdx_projection(Option_t* prjax ,const char* histoname = (const char*)0);
    TH2F* get_2d_av_dEdx_projection(Option_t* prjaxs,const char* histoname = (const char*)0);
   
    TH1F* get_1d_dEdx_D_projection(Option_t* prjax, const char* histoname = (const char*)0);
    TH2F* get_2d_dEdx_D_projection(Option_t* prjaxs,const char* histoname = (const char*)0);
    
    TH1F* get_1d_Dose_projection(Option_t* prjax, const char* histoname = (const char*)0);
    TH2F* get_2d_Dose_projection(Option_t* prjaxs,const char* histoname = (const char*)0);

    virtual ~dEdx_Calculator()
    {
        delete m_dedx_Edep_accumulator;
        delete m_Edep_accumulator;
        delete m_dedx_accumulator;
        delete m_nentries;
    };
};

#endif // DEDX_CALCULATOR_HH