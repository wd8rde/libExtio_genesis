#ifndef _GENESIS_GUI_H_
#define _GENESIS_GUI_H_
#include "genesis.h"

class GenesisGuiBase
{
public:
    GenesisGuiBase():mp_parent(NULL){};
    virtual ~GenesisGuiBase(){};
    virtual bool Init(Genesis* p_parent)
    {
        bool rtn = false;
        if (NULL != p_parent){
            mp_parent = p_parent;
            rtn = true;
        }
        return rtn;
    };
    virtual void SetAFPreamp(bool onoff) {};
    virtual bool HandleAFPreamp() {return true;};
protected:
    Genesis* mp_parent;
};
#endif /* _GENESIS_GUI_H_ */
