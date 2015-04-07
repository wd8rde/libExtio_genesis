#ifndef G59CMD_H_
#define G59CMD_H_

#include "usbutil.h"
#include "g59cmdpacket.h"

class G59Cmd
{
public:
    typedef enum
    {
        OK,
        FAILED_TO_SEND,
        BAD_ARG
    } tG59Err;

public:
    G59Cmd();
    virtual ~G59Cmd();
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
    tG59Err k_ratio(const int ratio);
    tG59Err pa10(const bool on_off);
    tG59Err line_mic(const bool on_off);
    tG59Err auto_cor(const bool on_off);
    tG59Err sec_rx2(const bool on_off);
    tG59Err monitor(const bool on_off);
protected:
    tG59Err private_set_freq(const long freq, const char* p_cmd);
    tG59Err private_send_on_off_cmd(const bool on_off, const char *p_on_cmd, const char *p_off_cmd);
    tG59Err private_cmd_arg2only(const unsigned char arg, const char *p_cmd);
    libusb_context* mp_ctx;
    unsigned char m_in_endpoint;
    unsigned char m_out_endpoint;
    libusb_device_handle *mp_dev_handle;
    int m_interface;

};
#endif /*G59CMD_H_*/
