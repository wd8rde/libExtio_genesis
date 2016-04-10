#include <stdio.h>
#import "genesis_observer.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}

Genesis_Observer::Genesis_Observer()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
Genesis_Observer::~Genesis_Observer()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnSetName()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnSetFreq()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnSmooth()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnAF(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnRF(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnATT(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnMUTE(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnTRV(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnSetFilt(int filter)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnTX(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnPA10(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnLineMic(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnPttCmd(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnAutoCOR(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnSecondRx(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnMonitor(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnKeySpeed()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnKeyMode()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnKeyRatio()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnDOT(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnDASH(bool on_off)
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}
void Genesis_Observer::OnIdle()
{
    LOG_INFO("%s:%d\n",__FUNCTION__,__LINE__);
}

