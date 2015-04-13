#ifndef EXTIO_H_
#define  EXTIO_H_
extern "C"
{
#include <stdbool.h>

void ShowGUI();
bool InitHW(char *name, char *model, int& type);
bool OpenHW();
void CloseHW();
int StartHW(long freq);
void StopHW();
int SetHWLO(long LOfreq);
int GetStatus();
void SetCallback(void (* Callback)(int, int, float, void *));
// ext routs
long GetHWLO();
void TuneChanged(long freq);
void IFLimitsChanged(long low, long high);
long GetTune();
char GetMode();
void ModeChanged(char mode);
long GetHWSR();
void HideGUI();
void RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples);
void GetFilters(int& loCut, int& hiCut, int& pitch);
//
int SetModeRxTx(int modeRxTx);
int ActivateTx(int magicA, int magicB);
void VersionInfo(const char *name, int ver_major, int ver_minor);
void SetPreamp(int db);
} /* extern "C" */
#endif /*  EXTIO_H_ */
