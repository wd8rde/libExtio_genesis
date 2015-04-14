#include <stdio.h>
#include <string.h>

#include "g59cmd.h"
#include "si570.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);}
//#define SIMULATE_USB_CONNECTION

G59Cmd::G59Cmd()
    : mp_ctx(NULL)
    , m_in_endpoint(0)
    , m_out_endpoint(0)
    , mp_dev_handle(NULL)
    , m_interface(0)
{
}

G59Cmd::~G59Cmd()
{
}

bool G59Cmd::Init(int vendor_id, int product_id)
{
    #ifdef SIMULATE_USB_CONNECTION
    return true;
    #endif
    bool rtn = false;
    libusb_init(&mp_ctx);
    mp_dev_handle = open_device(mp_ctx, vendor_id, product_id, &m_in_endpoint, &m_out_endpoint, &m_interface);
    if(NULL == mp_dev_handle)
    {
        LOG_ERR("%s:%d Error device not found\n",__FUNCTION__,__LINE__);
    }
    else
    {
        LOG_INFO("%s:%d Got endpoints(0x%02x,0x%02x)\n",__FUNCTION__,__LINE__, m_in_endpoint, m_out_endpoint);
        rtn = true;
    }

    return rtn;
}

bool G59Cmd::Close()
{
    #ifdef SIMULATE_USB_CONNECTION
    return true;
    #endif

    int rslt = close_device(mp_dev_handle, m_interface);
    return (0 == rslt);
}

G59Cmd::tG59Err G59Cmd::set_name(const char* name)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        G59CmdPacket::tconstG59Cmd cmd = "SET_NAME";
        G59CmdPacket::tconstG59Arg1 arg1 = "Genesis";

        G59CmdPacket packet(cmd, arg1);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(mp_dev_handle, m_out_endpoint, packet.GetPacket(), G59_PACKET_LEN);
        if(G59_PACKET_LEN == nBytes)
        {
           rtn = OK;
        }
    }
    else
    {
        LOG_ERR("%s:%d Error device is NULL\n",__FUNCTION__,__LINE__);
    }

    return rtn;
}

G59Cmd::tG59Err G59Cmd::private_set_freq(const long freq, const char* p_cmd)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        bool smooth = false;
        char arg1[G59_ARG1_LENGTH];
        memset(arg1, 0, G59_ARG1_LENGTH);

        char arg2[G59_ARG2_LENGTH];
        memset(arg2, 0, G59_ARG2_LENGTH);

        G59CmdPacket::tconstG59Cmd cmd = p_cmd;

        char freq_str[G59_ARG1_LENGTH+1];
        memset(freq_str, 0, G59_ARG1_LENGTH+1);
        snprintf(freq_str, G59_ARG1_LENGTH+1, "%08ld", freq);
        for(int i=0; i< G59_ARG1_LENGTH; i++)
        {
            arg1[i] = freq_str[i];
        }

        uint8_t regs[6];
        memset(regs, 0, sizeof(regs));
        uint32_t lo_freq = freq *4; //quadature mixer clock
        si570_set_frequency(lo_freq, regs);
        if (smooth)
        {
            cmd = "SMOOTH";
        }

        arg2[0x2]=0xaa; // set i2c address in first "register"
        for(int i=0; i<6; i++)
        {
            arg2[i+0x4]=regs[i];
        }

        G59CmdPacket packet(cmd, arg1, arg2);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(mp_dev_handle, m_out_endpoint, packet.GetPacket(), G59_PACKET_LEN);
        if(G59_PACKET_LEN == nBytes)
        {
           rtn = OK;
        }
    }
    else
    {
        LOG_ERR("%s:%d Error device is NULL\n",__FUNCTION__,__LINE__);
    }

    return rtn;
}

G59Cmd::tG59Err G59Cmd::set_freq(const long freq)
{
    return private_set_freq(freq, "SET_FREQ");
}

G59Cmd::tG59Err G59Cmd::smooth(const long freq)
{
    return private_set_freq(freq, "SMOOTH");
}

G59Cmd::tG59Err G59Cmd::private_send_on_off_cmd(const bool on_off, const char *p_on_cmd, const char *p_off_cmd)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        G59CmdPacket::tconstG59Cmd cmd;
        if (on_off)
        {
            cmd = p_on_cmd;
        }
        else
        {
            cmd = p_off_cmd;
        }

        G59CmdPacket packet(cmd);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(mp_dev_handle, m_out_endpoint, packet.GetPacket(), G59_PACKET_LEN);
        if(G59_PACKET_LEN == nBytes)
        {
           rtn = OK;
        }
    }
    else
    {
        LOG_ERR("%s:%d Error device is NULL\n",__FUNCTION__,__LINE__);
    }

    return rtn;
}
G59Cmd::tG59Err G59Cmd::af_amp(const bool on_off)
{
    return private_send_on_off_cmd(on_off, "AF_ON", "AF_OFF");
}

G59Cmd::tG59Err G59Cmd::rf_preamp(const bool on_off)
{
    return private_send_on_off_cmd(on_off, "RF_ON", "RF_OFF");
}

G59Cmd::tG59Err G59Cmd::att(const bool on_off)
{
    return private_send_on_off_cmd(on_off, "ATT_ON", "ATT_OFF");
}

G59Cmd::tG59Err G59Cmd::mute(const bool on_off)
{
    return private_send_on_off_cmd(on_off, "MUTE_ON", "MUTE_OFF");
}

G59Cmd::tG59Err G59Cmd::trv(const bool on_off)
{
    return private_send_on_off_cmd(on_off, "TRV_ON", "TRV_OFF");
}

G59Cmd::tG59Err G59Cmd::private_cmd_arg2only(const unsigned char arg, const char *p_cmd)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        char arg1[G59_ARG1_LENGTH];
        memset(arg1, 0, G59_ARG1_LENGTH);
        char arg2[G59_ARG2_LENGTH];
        memset(arg2, 0, G59_ARG2_LENGTH);

        G59CmdPacket::tconstG59Cmd cmd = p_cmd;
        arg2[0x04] = arg;

        G59CmdPacket packet(cmd, arg1, arg2);
        packet.DumpPacket();

        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(mp_dev_handle, m_out_endpoint, packet.GetPacket(), G59_PACKET_LEN);
        if(G59_PACKET_LEN == nBytes)
        {
           rtn = OK;
        }
    }
    else
    {
        LOG_ERR("%s:%d Error device is NULL\n",__FUNCTION__,__LINE__);
    }

    return rtn;
}

G59Cmd::tG59Err G59Cmd::set_filt(const int fltr)
{
    return private_cmd_arg2only((fltr & 0xff),"SET_FILT");
}

G59Cmd::tG59Err G59Cmd::tx(const bool on_off)
{
    tG59Err rtn = OK;
    if (on_off)
    {
        rtn = private_cmd_arg2only(0x03,"TX_ON");
    }
    else
    {
        rtn = private_cmd_arg2only(0x00,"TX_OFF");
    }
    return rtn;
}

G59Cmd::tG59Err G59Cmd::pa10(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff),"PA10_ON");
}

G59Cmd::tG59Err G59Cmd::line_mic(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff),"LINE/MIC");
}

G59Cmd::tG59Err G59Cmd::auto_cor(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff),"AUTO_COR");
}

G59Cmd::tG59Err G59Cmd::sec_rx2(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff),"SEC_RX2");
}

G59Cmd::tG59Err G59Cmd::monitor(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff),"MONITOR");
}

G59Cmd::tG59Err G59Cmd::k_speed(const int wpm)
{
    if ((7 > wpm)||(128 < wpm))
    {
        return BAD_ARG;
    }
    unsigned char divisor = (unsigned char)((divisor/wpm) & 0xff);
    return private_cmd_arg2only(divisor, "K_SPEED");
}

G59Cmd::tG59Err G59Cmd::k_mode(const int mode)
{
    return private_cmd_arg2only((unsigned char)(mode & 0xff), "K_MODE");
}

G59Cmd::tG59Err G59Cmd::k_ratio(const int ratio)
{
    return private_cmd_arg2only((unsigned char)(ratio & 0xff), "K_RATIO");
}

