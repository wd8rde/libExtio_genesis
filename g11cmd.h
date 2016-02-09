#ifndef G11CMD_H_
#define G11CMD_H_

#include "cmdbase.h"

class G11Cmd : public CmdBase
{
    public:

        G11Cmd() {};
        virtual ~G11Cmd() {};
        virtual const CmdBase::t_cmdinfo get_cmd_info(CmdBase::t_cmd_enum cmd)
        {
            return cmdinfo[cmd];
        }

    protected:
        static const CmdBase::t_cmdinfo cmdinfo[];
};

const CmdBase::t_cmdinfo G11Cmd::cmdinfo[] =
{
    {CmdBase::NONE,"NONE"},
    {CmdBase::SET_NAME,"SET_NAME"},
    {CmdBase::SET_FREQ,"SET_FREQ"},
    {CmdBase::SMOOTH,  "SMOOTH"},
    {CmdBase::AF_ON,   "AF_ON"},
    {CmdBase::AF_OFF,  "AF_OFF"},
    {CmdBase::RF_ON,   "RF_ON"},
    {CmdBase::RF_OFF,  "RF_OFF"},
    {CmdBase::ATT_ON,  "ATT_ON"},
    {CmdBase::ATT_OFF, "ATT_OFF"},
    {CmdBase::MUTE_ON, "MUTE_ON"},
    {CmdBase::MUTE_OFF,"MUTE_OFF"},
    {CmdBase::TRV_ON,  "TRV_ON"},
    {CmdBase::TRV_OFF, "TRV_OFF"},
    {CmdBase::SET_FILT,"B_FILTER"},
    {CmdBase::TX_ON,   "TX_ON"},
    {CmdBase::TX_OFF,  "TX_OFF"},
    {CmdBase::PA10_ON, "PA10_ON"},
    {CmdBase::LINE_MIC,"LINE/MIC"},
    {CmdBase::AUTO_COR,"AUTO_COR"},
    {CmdBase::SEC_RX2, "SEC_RX2"},
    {CmdBase::MONITOR, "MONITOR"},
    {CmdBase::K_SPEED, "K_SPEED"},
    {CmdBase::K_MODE,  "K_MODE"},
    {CmdBase::K_RATIO, "K_RATIO"},
    {CmdBase::DOT_ON,  "DOT_ON"},
    {CmdBase::DOT_OFF, "DOT_OFF"},
    {CmdBase::DASH_ON, "DASH_ON"},
    {CmdBase::DASH_OFF,"DASH_OFF"},
    {CmdBase::PWR_SWR, "PWR_SWR"},
    {CmdBase::IDLE,"IDLE"}
};

#endif /* G11CMD_H_ */
