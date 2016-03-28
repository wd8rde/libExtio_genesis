#ifndef GENESIS_OBSERVER_H
#define GENESIS_OBSERVER_H
class Genesis_Observer
{
public:
    Genesis_Observer();
    virtual ~Genesis_Observer();
    virtual void OnSetName();
    virtual void OnSetFreq();
    virtual void OnSmooth();
    virtual void OnAF(bool on_off);
    virtual void OnRF(bool on_off);
    virtual void OnATT(bool on_off);
    virtual void OnMUTE(bool on_off);
    virtual void OnTRV(bool on_off);
    virtual void OnSetFilt(int filter);
    virtual void OnTX(bool on_off);
    virtual void OnPA10(bool on_off);
    virtual void OnLineMic(bool on_off);
    virtual void OnAutoCOR(bool on_off);
    virtual void OnSecondRx(bool on_off);
    virtual void OnMonitor(bool on_off);
    virtual void OnKeySpeed();
    virtual void OnKeyMode();
    virtual void OnKeyRatio();
    virtual void OnDOT(bool on_off);
    virtual void OnDASH(bool on_off);
    virtual void OnIdle();
};
#endif /*GENESIS_OBSERVER_H*/

