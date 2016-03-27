#ifndef EXTIO_H_
#define  EXTIO_H_
extern "C"
{
#include <stdbool.h>
enum ExtIOCallbackStatus {
    // for all extIO's
    ecsChangedSampleRate    = 100   // sampling speed has changed in the external HW
    , ecsChangedLO          = 101   // LO frequency has changed in the external HW
    , ecsLockLO             = 102
    , ecsUnlockLO           = 103
    , ecsChangedLONotTUNE   = 104   // CURRENTLY NOT YET IMPLEMENTED
    // LO freq. has changed, Winrad must keep the Tune freq. unchanged
    // (must immediately call GetHWLO() )
    , ecsChangedTUNE        = 105   // a change of the Tune freq. is being requested.
    // Winrad must call GetTune() to know which value is wanted
    , ecsChangedMODE        = 106   // a change of demod. mode is being requested.
    // Winrad must call GetMode() to know the new mode
    , ecsStart              = 107   // The DLL wants Winrad to Start
    , ecsStop               = 108   // The DLL wants Winrad to Stop
    , ecsChangedFILTER      = 109   // a change in the band limits is being requested
    // Winrad must call GetFilters()

    , ecsTXRequest          = 120   // DLL requests TX mode / User pressed PTT
    //   exciter/transmitter must wait until SetModeRxTx() is called!
    , ecsRXRequest          = 121   // DLL wants to leave TX mode / User released PTT
    //   exciter/transmitter must wait until SetModeRxTx() is called!

    , ecsDotOnRequest       = 520   // DLL requests Dot start mode / User pressed Dot Paddle or Straight key
    , ecsDotOffRequest      = 521   // DLL wants to leave Dot start mode / User released Dot Paddle or Straight key
    , ecsDashOnRequest      = 522   // DLL requests Dash start mode / User pressed Dash Paddle or Straight key
    , ecsDashOffRequest     = 523   // DLL wants to leave Dash start mode / User released Dash Paddle or Straight key
};
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
// Genesis routs
void EnableLineMic(int onoff);
void EnablePA(int onoff);
void SetKeyerRatio(float ratio_dot_to_dash);
void SetKeyerMode(int keyer_mode);
void SetWpm(int wpm);

//
int SetModeRxTx(int modeRxTx);
int ActivateTx(int magicA, int magicB);
void VersionInfo(const char *name, int ver_major, int ver_minor);
void SetPreamp(int db);
} /* extern "C" */
#endif /*  EXTIO_H_ */
