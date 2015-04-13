#include <string.h>
#include <string>
#include <stdio.h>
#include "bsd_string.h"

#include "genesis.h"
#include "g59cmd.h"


static G59 m_g59;
static G11 m_g11;
static Genesis *mp_genesis = NULL;

static char m_name_str[256];
static char m_model_str[256];
static char m_mode = '\0';
static long m_tune_freq = 1800000;
static long m_lo_freq = 1800000;

extern "C"
{
//Foward Decl
int SetHWLO(long LOfreq);

void ShowGUI()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
bool InitHW(char *name, char *model, int& type)
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    bool rtn = false;
    type = 0;
    name = NULL;
    model = NULL;
    if (rtn = m_g59.Init())
    {
        mp_genesis = &m_g59;
        strlcpy(m_name_str, m_g59.GetMake().c_str(), sizeof(m_name_str));
        strlcpy(m_model_str, m_g59.GetMake().c_str(), sizeof(m_model_str));
        name = m_name_str;
        model = m_model_str;
        type = 4;
    }
    else if (rtn = m_g11.Init())
    {
        mp_genesis = &m_g11;
        strlcpy(m_name_str, m_g11.GetMake().c_str(), sizeof(m_name_str));
        strlcpy(m_model_str, m_g11.GetMake().c_str(), sizeof(m_model_str));
        name = m_name_str;
        model = m_model_str;
        type = 4;
    }

    return rtn;
}
bool OpenHW()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    SetHWLO(m_lo_freq);
    return true;
}
void CloseHW()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    mp_genesis->Close();
}
int StartHW(long freq)
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}
void StopHW()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
int SetHWLO(long LOfreq)
{
    fprintf(stderr,"%s:%d LOfreq: %ld\n",__FUNCTION__,__LINE__, LOfreq);
    mp_genesis->SetLO(LOfreq);
    m_lo_freq = LOfreq;
    return 0;
}
int GetStatus()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}
void SetCallback(void (* Callback)(int, int, float, void *))
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
// ext routs
long GetHWLO()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_lo_freq;
}
void TuneChanged(long freq)
{
    fprintf(stderr,"%s:%d freq: %ld\n",__FUNCTION__,__LINE__, freq);
    mp_genesis->SetBand(freq);
    m_tune_freq = freq;
}
void IFLimitsChanged(long low, long high)
{
    long LOfreq = (low + high)/2;
    fprintf(stderr,"%s:%d low: %ld, high: %ld, LO = %ld\n",__FUNCTION__,__LINE__, low, high, LOfreq);
    if((0 < LOfreq) && (LOfreq != m_lo_freq))
    {
        mp_genesis->SetLO(LOfreq);
        m_lo_freq = LOfreq;
    }

}
long GetTune()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_tune_freq;
}
char GetMode()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return m_mode;
}
void ModeChanged(char mode)
{
    m_mode = mode;
    fprintf(stderr,"%s:%d %c\n",__FUNCTION__,__LINE__,mode);
}
long GetHWSR()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
    return 0;
}
void HideGUI()
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
void RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples)
{
    fprintf(stderr,"%s:%d samprate:%ld, numsamples:%d\n",__FUNCTION__,__LINE__, samprate, numsamples);
}
void GetFilters(int& loCut, int& hiCut, int& pitch)
{
    fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
}
//
int SetModeRxTx(int modeRxTx)
{
    fprintf(stderr,"%s:%d modeRxTx: %d\n",__FUNCTION__,__LINE__, modeRxTx);
    mp_genesis->SetTx((0!=modeRxTx?true:false));
    return 0;
}
int ActivateTx(int magicA, int magicB)
{
    fprintf(stderr,"%s:%d magicA:%d, int magicB:%d\n",__FUNCTION__,__LINE__,magicA, magicB);
    return 0;
}
void VersionInfo(const char *name, int ver_major, int ver_minor)
{
    fprintf(stderr,"%s:%d name:\"%s\", ver_major:%d, ver_minor:%d\n",__FUNCTION__,__LINE__, name, ver_major, ver_minor);
}
void SetAtten(int db)
{
    fprintf(stderr,"%s:%d db=%d\n",__FUNCTION__,__LINE__, db);
    if (0 > db)
    {
        mp_genesis->SetAtten(true);
        mp_genesis->SetRFPreamp(false);
    }
    else if ( 0 < db)
    {
        mp_genesis->SetAtten(false);
        mp_genesis->SetRFPreamp(true);
    }
    else
    {
        mp_genesis->SetAtten(false);
        mp_genesis->SetRFPreamp(false);
    }
}
} /* extern "C" */
