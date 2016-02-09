#ifndef CMDBASE_H_
#define CMDBASE_H_

#include "hid_util.h"
#include "cmdpacket.h"
#include "genesis_observer.h"

class CmdBase
{
public:
    typedef enum
    {
        OK,
        FAILED_TO_SEND,
        BAD_ARG
    } tGenesisErr;


    typedef struct tGenesisThreadData
    {
      CmdBase* p_instance;
      // someday, may want to expand this
    } tGenesisThreadData;

    typedef enum
    {
        NONE = 0,
        SET_NAME,
        SET_FREQ,
        SMOOTH,
        AF_ON,
        AF_OFF,
        RF_ON,
        RF_OFF,
        ATT_ON,
        ATT_OFF,
        MUTE_ON,
        MUTE_OFF,
        TRV_ON,
        TRV_OFF,
        SET_FILT,
        TX_ON,
        TX_OFF,
        PA10_ON,
        LINE_MIC,
        AUTO_COR,
        SEC_RX2,
        MONITOR,
        K_SPEED,
        K_MODE,
        K_RATIO,
        DOT_ON,
        DOT_OFF,
        DASH_ON,
        DASH_OFF,
        PWR_SWR,
        IDLE,
        LAST_CMD
    }t_cmd_enum;

    typedef struct
    {
        const CmdBase::t_cmd_enum cmd;
        const char* cmd_str;
    }t_cmdinfo;

    typedef enum
    {
        K_MODE_NONE         = 0x00,
        K_MODE_IAMBIC       = 0x01,
        K_MODE_KEYER_MODE_B = 0x02,
        K_MODE_REV_PADDLE   = 0x04
    }t_k_mode;

public:
    CmdBase();
    virtual ~CmdBase();
    void register_observer(Genesis_Observer *p_observer);
    bool Init(int vendor_id, int product_id);
    bool Close();
    tGenesisErr set_name(const char* name);
    tGenesisErr set_freq(const long freq);
    tGenesisErr smooth(const long  freq);
    tGenesisErr set_filt(const int fltr);
    tGenesisErr af_amp(const bool on_off);
    tGenesisErr rf_preamp(const bool on_off);
    tGenesisErr att(const bool on_off);
    tGenesisErr mute(const bool on_off);
    tGenesisErr trv(const bool on_off);
    tGenesisErr tx(const bool on_off);
    tGenesisErr k_speed(const int wpm);
    tGenesisErr k_mode(const int mode);
    tGenesisErr k_ratio(const double ratio);
    tGenesisErr pa10(const bool on_off);
    tGenesisErr line_mic(const bool on_off);
    tGenesisErr auto_cor(const bool on_off);
    tGenesisErr sec_rx2(const bool on_off);
    tGenesisErr monitor(const bool on_off);
    void* usb_read_thread_func();
    void set_tx_dropout_ms(unsigned long tx_dropout_ms);

protected:
    typedef enum
    {
        TX_STATE_IGNORE = 0,
        TX_STATE_ON = 1,
        TX_STATE_DROPPING = 2
    }t_tx_state;

    virtual const t_cmdinfo get_cmd_info(CmdBase::t_cmd_enum cmd);

    bool init_usb_read_thread();
    tGenesisErr private_set_freq(const long freq, const char* p_cmd);
    tGenesisErr private_send_on_off_cmd(const bool on_off, const char *p_on_cmd, const char *p_off_cmd);
    tGenesisErr private_cmd_arg2only(const unsigned char arg, const char *p_cmd);
    t_cmd_enum private_parse_packet(CmdPacket &packet);
    const t_cmd_enum private_str2cmd(std::string cmd);
    t_tx_state private_handle_cmd(t_cmd_enum cmd, CmdPacket &packet);

    hid_handle m_dev_handle;
    pthread_t m_usb_read_thread;
    bool m_usb_read_thread_running;
    pthread_mutex_t m_usb_read_mutex;
    Genesis_Observer* mp_observer;
    long long m_tx_dropout_timeout_ns;

private:
    static void* static_thread_func(void* p_data);
    static const t_cmdinfo cmdinfo[];


};
#endif /*CMDBASE_H_*/
