#include "FlukaDumpReader.hh"

int FlukaDumpReader::Analyse(FlukaDumpAnalyser *fda, int events_to_analyse)
{
    int first_rec = 0;
    int vv = 0;
    FlukaPartTrack *beam_part = nullptr;
    fda->set_active_beam_track(beam_part);
    for (vv = 0; get_first_int(&first_rec) == sizeof(int) && (events_to_analyse < 0 || vv < events_to_analyse);)
    {
        //std::cout <<"ntrk " <<ntrk<<" case2 "<<case2<<" case3 "<<case3<< std::endl;
        if (first_rec > 0)
        {
            bool is_beam_particle = false;
            first_rec_case_1 rec;
            read_first_rec(first_rec, &rec);
            SimpleFlukaTrack *trk = get_SimpleFlukaTrack(&rec);
            if (beam_part != nullptr)
            {
                is_beam_particle = beam_part->PushBackSimpleFlukaTrack(trk, rec.JTRACK, rec.ETRACK);
            }
            fda->Analyse_Track_Record(rec, trk, is_beam_particle);
            delete trk;
        }
        else if (first_rec < 0)
        {
            vv++;
            first_rec_case_3 rec;
            read_first_rec(first_rec, &rec);
            Case3Buffer *c3b = get_Case3Buffer(&rec);
            if (beam_part != nullptr)
            {
                //    beam_part->print();
                fda->push_back_beam_track(beam_part);
                beam_part = nullptr;
                fda->set_active_beam_track(beam_part);
            }

            beam_part = new FlukaPartTrack(c3b->data[0].ILOFLK,
                                           c3b->data[0].XFLK,
                                           c3b->data[0].YFLK,
                                           c3b->data[0].ZFLK,
                                           c3b->data[0].ETOT);
            fda->set_active_beam_track(beam_part);
            fda->Analyse_Primary_Record(rec, c3b);
            delete c3b;
        }
        else
        {
            first_rec_case_2 rec;
            read_first_rec(&rec);
            rec_case_2 rec_data;
            get_Case2Buffer(&rec, &rec_data);
            fda->Analyse_Energy_Record(rec, rec_data);
            if (beam_part != nullptr && beam_part->ParticleID_JTRACK == rec.JTRACK)
            {
                auto last_point = beam_part->get_last_point();
                if (last_point.x == rec_data.XSCO && last_point.y == rec_data.YSCO && last_point.z == rec_data.ZSCO)
                    if (rec.ICODE == 14 || rec.ICODE == 23 || rec.ICODE == 32 || rec.ICODE == 40 || rec.ICODE == 52 || rec.ICODE == 12 || rec.ICODE == 11 || rec.ICODE == 13)
                    {
                        //beam_part->print();
                        beam_part->EndTrackICODE = rec.ICODE;
                        fda->push_back_beam_track(beam_part);
                        beam_part = nullptr;
                        fda->set_active_beam_track(beam_part);
                        //std::cout<<"end primary icode "<<rec.ICODE<<" ETRACK "<<rec.ETRACK<<" RULL "<<rec_data.RULL<<std::endl;
                        if (fda->get_PrimaryParticlesContainer().size()%100==0)
                        {
                            std::cout<<"Primary particles proccessed: "<<fda->get_PrimaryParticlesContainer().size()<<std::endl;
                        }
                    }
            }
        }
    }
    return vv;
}

const int RECORD_DELIMITER_LENGTH = 4;
// Case 1 (First variable > 0 ): continuous energy deposition
// Case 2 (First variable = 0 ): point energy deposition
// Case 3 (First variable < 0 ): source particles
int FlukaDumpReader::get_first_int(int *res)
{

    if (!dumpfile->eof())
    {
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        int n = dumpfile->readsome(reinterpret_cast<char *>(res), sizeof(int));
        //std::cout<<"first int read bytes "<<n<<std::endl;
        return n;
    }
    else
        throw "Unsuccessful get_first_int";
    return -1;
}

void FlukaDumpReader::read_first_rec(int NTRACK, first_rec_case_1 *rec)
{
    if (!dumpfile->eof())
    {
        int intbuff[2];
        float floatbuff[2];
        dumpfile->read((char *)intbuff, 2 * sizeof(int));
        dumpfile->read((char *)floatbuff, 2 * sizeof(float));
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        //		dumpfile->read((char*)&(rec->MTRACK),sizeof(first_rec_case_1)-sizeof(int));
        rec->NTRACK = NTRACK;
        rec->MTRACK = intbuff[0];
        rec->JTRACK = intbuff[1];
        rec->ETRACK = floatbuff[0];
        rec->WTRACK = floatbuff[1];

    }
    else
        throw "Unsuccessful read_first_rec_case1";
}

void FlukaDumpReader::read_first_rec(first_rec_case_2 *rec)
{
    if (!dumpfile->eof())
    {
        dumpfile->read((char *)&(rec->ICODE), sizeof(first_rec_case_2) - sizeof(int));
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        rec->ZERO = 0;
    }
    else
        throw "Unsuccessful read_first_rec_case2";
}

void FlukaDumpReader::read_first_rec(int mNCASE, first_rec_case_3 *rec)
{
    if (!dumpfile->eof())
    {
 //       dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        dumpfile->read((char *)&(rec->NPFLKA), sizeof(first_rec_case_3) - sizeof(int));
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        rec->NCASE = -mNCASE;
    }
    else
        throw "Unsuccessful read_first_rec_case3";
}

SimpleFlukaTrack *FlukaDumpReader::get_SimpleFlukaTrack(first_rec_case_1 *rec)
{
    if (!dumpfile->eof())
    {
        SimpleFlukaTrack *trck = new SimpleFlukaTrack(rec->NTRACK, rec->MTRACK);
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        dumpfile->read((char *)trck->Segmets_ends, trck->get_segmets_memory());
        dumpfile->read((char *)trck->energy_deposition_events, trck->get_enegry_deposition_memory());
        dumpfile->read((char *)&trck->total_curved_path, trck->get_total_curved_path_memory());
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        return trck;
    }
    else
        throw "Unsuccessful get_SimpleFlukaTrack";
    return NULL;
}

Case3Buffer *FlukaDumpReader::get_Case3Buffer(first_rec_case_3 *rec)
{
    if (!dumpfile->eof())
    {
        Case3Buffer *c3b = new Case3Buffer(rec);
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        dumpfile->read((char *)c3b->data, c3b->get_buffer_memory());
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        return c3b;
    }
    else
        throw "Unsuccessful get_Case3Buffer";
    return NULL;
}

void FlukaDumpReader::get_Case2Buffer(first_rec_case_2 *first_rec, rec_case_2 *rec)
{
    if (!dumpfile->eof())
    {
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
        dumpfile->read((char *)rec, sizeof(rec_case_2));
        dumpfile->seekg(RECORD_DELIMITER_LENGTH, std::ios::cur);
    }
    else
        throw "Unsuccessful get_Case3Buffer";
}
