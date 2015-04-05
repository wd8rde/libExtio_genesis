#include <string.h>
#include <string>

#include "genesis.h"
#include "g59cmd.h"

G59 m_g59;

extern "C"
{
void ShowGUI()
{
}
bool InitHW(char *name, char *model, int& type)
{
    bool rtn = false;
    type = 0;
    name = NULL;
    model = NULL;
    if (rtn = m_g59.Init())
    {
        name = strdup(m_g59.GetMake().c_str());
        model = strdup(m_g59.GetModel().c_str());
        type = 4;
    }

    return rtn;
}
bool OpenHW()
{
    return true;
}
void CloseHW()
{
}
int StartHW(long freq)
{
}
void StopHW()
{
}
int SetHWLO(long LOfreq)
{
}
int GetStatus()
{
}
void SetCallback(void (* Callback)(int, int, float, void *))
{
}
// ext routs
long GetHWLO()
{
}
void TuneChanged(long freq)
{
}
void IFLimitsChanged(long low, long high)
{
}
long GetTune()
{
}
char GetMode()
{
}
void ModeChanged(char mode)
{
}
long GetHWSR()
{
}
void HideGUI()
{
}
void RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples)
{
}
void GetFilters(int& loCut, int& hiCut, int& pitch)
{
}
//
int SetModeRxTx(int modeRxTx)
{
}
int ActivateTx(int magicA, int magicB)
{
}
void VersionInfo(const char *name, int ver_major, int ver_minor)
{
}
} /* extern "C" */
