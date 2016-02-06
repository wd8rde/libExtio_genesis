#ifndef G59CMD_H_
#define G59CMD_H_

#include "hid_util.h"
#include "g59cmdpacket.h"
#include "genesis_observer.h"

class G59Cmd
{
public:
    typedef enum
    {
        OK,
        FAILED_TO_SEND,
        BAD_ARG
    } tG59Err;


    typedef struct tG59ThreadData
    {
      G59Cmd* p_instance;
      // someday, may want to expand this
    } tG59ThreadData;

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

    typedef enum
    {
        K_MODE_NONE         = 0x00,
        K_MODE_IAMBIC       = 0x01,
        K_MODE_KEYER_MODE_B = 0x02,
        K_MODE_REV_PADDLE   = 0x04
    }t_k_mode;

public:
    G59Cmd();
    virtual ~G59Cmd();
    void register_observer(Genesis_Observer *p_observer);
    bool Init(int vendor_id, int product_id);
    bool Close();
    tG59Err set_name(const char* name);
    tG59Err set_freq(const long freq);
    tG59Err smooth(const long  freq);
    tG59Err set_filt(const int fltr);
    tG59Err af_amp(const bool on_off);
    tG59Err rf_preamp(const bool on_off);
    tG59Err att(const bool on_off);
    tG59Err mute(const bool on_off);
    tG59Err trv(const bool on_off);
    tG59Err tx(const bool on_off);
    tG59Err k_speed(const int wpm);
    tG59Err k_mode(const int mode);
    tG59Err k_ratio(const double ratio);
    tG59Err pa10(const bool on_off);
    tG59Err line_mic(const bool on_off);
    tG59Err auto_cor(const bool on_off);
    tG59Err sec_rx2(const bool on_off);
    tG59Err monitor(const bool on_off);
    void* usb_read_thread_func();
    void set_tx_dropout_ms(unsigned long tx_dropout_ms);

protected:
    typedef enum
    {
        TX_STATE_IGNORE = 0,
        TX_STATE_ON = 1,
        TX_STATE_DROPPING = 2
    }t_tx_state;

    bool init_usb_read_thread();
    tG59Err private_set_freq(const long freq, const char* p_cmd);
    tG59Err private_send_on_off_cmd(const bool on_off, const char *p_on_cmd, const char *p_off_cmd);
    tG59Err private_cmd_arg2only(const unsigned char arg, const char *p_cmd);
    t_cmd_enum private_parse_packet(G59CmdPacket &packet);
    const t_cmd_enum private_str2cmd(std::string cmd);
    t_tx_state private_handle_cmd(t_cmd_enum cmd, G59CmdPacket &packet);

    hid_handle m_dev_handle;
    pthread_t m_usb_read_thread;
    bool m_usb_read_thread_running;
    pthread_mutex_t m_usb_read_mutex;
    Genesis_Observer* mp_observer;
    long long m_tx_dropout_timeout_ns;

private:
    static void* static_thread_func(void* p_data);


};
#endif /*G59CMD_H_*/
