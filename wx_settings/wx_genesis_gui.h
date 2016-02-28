#ifndef WX_GENESIS_GUI
#define WX_GENESIS_GUI
#include "genesis_guibase.h"
class wxGenesisGui : public GenesisGuiBase{
public:
    wxGenesisGui();
    virtual ~wxGenesisGui();
    virtual void SetAFPreamp(bool onoff);
    virtual bool HandleAFPreamp();
};
#endif /* WX_GENESIS_GUI */

