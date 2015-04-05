#include <stdio.h>
#include <string.h>

#include "g59cmd.h"
#include "si570.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);}

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
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        bool smooth = false;
        char arg1[G59_ARG1_LENGTH];
        memset(arg1, 0, G59_ARG1_LENGTH);

        char arg2[G59_ARG2_LENGTH];
        memset(arg2, 0, G59_ARG2_LENGTH);

        G59CmdPacket::tconstG59Cmd cmd = "SET_FREQ";

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

G59Cmd::tG59Err G59Cmd::smooth(const long freq)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::set_filt(const int fltr)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        char arg1[G59_ARG1_LENGTH];
        memset(arg1, 0, G59_ARG1_LENGTH);
        char arg2[G59_ARG2_LENGTH];
        memset(arg2, 0, G59_ARG1_LENGTH);

        G59CmdPacket::tconstG59Cmd cmd = "SET_FILT";

        arg2[0x4] = (fltr & 0xff);

        G59CmdPacket packet(cmd, arg1, arg2);
        packet.DumpPacket();

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
    return OK;
}

G59Cmd::tG59Err G59Cmd::rf_preamp(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::att(const bool on_off)
{
    G59Cmd::tG59Err rtn = FAILED_TO_SEND;
    if (NULL != mp_dev_handle)
    {
        G59CmdPacket::tconstG59Cmd cmd;
        if (on_off)
        {
            cmd = "ATT_ON";
        }
        else
        {
            cmd = "ATT_OFF";
        }

        G59CmdPacket packet(cmd);
        packet.DumpPacket();
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

G59Cmd::tG59Err G59Cmd::mute(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::trv(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::tx(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::k_speed(const float wpm)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::k_mode(const int mode)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::k_ratio(const int ratio)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::pa10(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::line_mic(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::auto_cor()
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::sec_rx2(const bool on_off)
{
    return OK;
}

G59Cmd::tG59Err G59Cmd::monitor()
{
    return OK;
}

