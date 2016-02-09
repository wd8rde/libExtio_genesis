#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <string>
#include <algorithm>
#include <time.h>

#include "cmdbase.h"
#include "si570.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#ifdef DO_ANNOYING
#define LOG_ANNOYING(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#else
#define LOG_ANNOYING(...) {}
#endif

//#define SIMULATE_USB_CONNECTION

#define TIMER_CLOCK_ID CLOCK_MONOTONIC
#define TSPEC2LL(ts) (static_cast<long long>(ts.tv_sec*NSECS_IN_SEC) + ts.tv_nsec)

const bool ON = true;
const bool OFF = false;

static const long long NSECS_IN_SEC = 1000000000;
const long NSEC_PER_MILLISEC = 1000000;
const long long DEFAULT_IDLE_TIMEOUT = 300 * NSEC_PER_MILLISEC;

const CmdBase::t_cmdinfo CmdBase::cmdinfo[] =
{
    {CmdBase::NONE,"NONE"}
};

CmdBase::CmdBase()
    : m_dev_handle(NULL)
    , m_usb_read_thread_running(false)
    , mp_observer(NULL)
    , m_tx_dropout_timeout_ns(DEFAULT_IDLE_TIMEOUT)
{
    pthread_mutex_init(&m_usb_read_mutex, NULL);
}

CmdBase::~CmdBase()
{
    pthread_mutex_destroy(&m_usb_read_mutex);
}

bool CmdBase::Init(int vendor_id, int product_id)
{
    LOG_INFO("%s:%d  vid:%x, pid:%x\n",__FUNCTION__,__LINE__,vendor_id,product_id);
    #ifdef SIMULATE_USB_CONNECTION
    return true;
    #endif

    bool rtn = false;
    m_dev_handle = open_device(vendor_id, product_id, NULL);
    if(NULL == m_dev_handle)
    {
        LOG_ERR("%s:%d Error vid:%x, pid:%x not found\n",__FUNCTION__,__LINE__,vendor_id,product_id);
    }
    else
    {
        rtn = init_usb_read_thread();
    }

    return rtn;
}

bool CmdBase::Close()
{
    bool rtn = true;
    #ifdef SIMULATE_USB_CONNECTION
    return true;
    #endif

    //Stop usb read thread
    m_usb_read_thread_running = false;
    LOG_INFO("%s:%d Stopping usb read thread\n",__FUNCTION__,__LINE__);
    void* thread_exit_result;
    pthread_join(m_usb_read_thread, &thread_exit_result);
    LOG_INFO("%s:%d usb read thread has stopped\n",__FUNCTION__,__LINE__);

    if( NULL != m_dev_handle)
    {
        close_device(m_dev_handle);

        m_dev_handle = NULL;
    }
    return rtn;
}

const CmdBase::t_cmdinfo CmdBase::get_cmd_info(CmdBase::t_cmd_enum cmd)
{
    return cmdinfo[NONE];
}

bool CmdBase::init_usb_read_thread()
{
    bool rtn = true;

    m_usb_read_thread_running = true;
    int rslt = pthread_create(&m_usb_read_thread, 
                              NULL /* pthread_attr_t *attr*/, 
                              &CmdBase::static_thread_func, 
                              this);
    if (0 != rslt)
    {
        LOG_ERR("%s:%d pthread_create failed %d, %s\n",__FUNCTION__,__LINE__, rslt, strerror(rslt));
        rtn = false;
    }

    return rtn;
}

void* CmdBase::static_thread_func(void* p_data)
{
    LOG_INFO("%s:%d Starting\n",__FUNCTION__,__LINE__);
    if(NULL != p_data)
    {
        CmdBase* p_inst = static_cast<CmdBase*>(p_data);
        p_inst->usb_read_thread_func();
    }
    LOG_INFO("%s:%d Exiting\n",__FUNCTION__,__LINE__);
    return NULL;
}

void* CmdBase::usb_read_thread_func()
{
    LOG_ANNOYING("%s:%d Entering thread loop\n",__FUNCTION__,__LINE__);
    struct timespec current_time;
    clock_gettime(TIMER_CLOCK_ID, &current_time);

    // initialize tx drop out timer
    bool timer_running = false;
    struct timespec previous_time;
    previous_time.tv_sec = current_time.tv_sec;
    previous_time.tv_nsec = current_time.tv_nsec;
    long long elapsed_time = (TSPEC2LL(previous_time) - TSPEC2LL(current_time));

    t_cmd_enum last_cmd = NONE;

    unsigned char rx_packet[GENESIS_PACKET_LEN];
    while(m_usb_read_thread_running)
    {
        if (NULL != m_dev_handle)
        {
            clock_gettime(TIMER_CLOCK_ID, &current_time);
            memset(rx_packet,0,GENESIS_PACKET_LEN);
            int rslt = read_from_device(m_dev_handle, reinterpret_cast<unsigned char*>(rx_packet), GENESIS_PACKET_LEN);
            if(0 < rslt)
            {
                CmdPacket genesis_packet(reinterpret_cast<CmdPacket::tGenesisPacket>(rx_packet));
                CmdBase::t_cmd_enum cmd = private_parse_packet(genesis_packet);
                if (cmd != last_cmd)
                {
                    // handle_cmd returns true if tx active
                    CmdBase::t_tx_state tx_state = private_handle_cmd(cmd, genesis_packet);
                    switch (tx_state)
                    {
                    case TX_STATE_ON:
                        //stop and reset timer
                        LOG_ANNOYING("%s:%d timer off\n",__FUNCTION__,__LINE__);
                        timer_running = false;
                        clock_gettime(TIMER_CLOCK_ID, &current_time);
                        previous_time.tv_sec = current_time.tv_sec;
                        previous_time.tv_nsec = current_time.tv_nsec;
                        break;
                    case TX_STATE_DROPPING:
                        //start timer
                        LOG_ANNOYING("%s:%d timer on\n",__FUNCTION__,__LINE__);
                        timer_running = true;
                        clock_gettime(TIMER_CLOCK_ID, &current_time);
                        previous_time.tv_sec = current_time.tv_sec;
                        previous_time.tv_nsec = current_time.tv_nsec;
                        break;
                    case TX_STATE_IGNORE:
                    default:
                        LOG_ANNOYING("%s:%d timer %s\n",__FUNCTION__,__LINE__,(timer_running?"true":"false"));
                        break;
                    }
                    last_cmd = cmd;
                }
            }

            if (timer_running)
            {
                elapsed_time = ( TSPEC2LL(current_time) - TSPEC2LL(previous_time) );
                //LOG_INFO("%s:%d timer elapsed=%lld\n",__FUNCTION__,__LINE__,elapsed_time);
                if (m_tx_dropout_timeout_ns < elapsed_time)
                {
                    LOG_ANNOYING("%s:%d timer expired\n",__FUNCTION__,__LINE__);
                    // Been idle too long
                    timer_running = false;

                    char arg1[GENESIS_ARG1_LENGTH];
                    memset(arg1, 0, GENESIS_ARG1_LENGTH);
                    char arg2[GENESIS_ARG2_LENGTH];
                    memset(arg2, 0, GENESIS_ARG2_LENGTH);
                    CmdBase::t_cmd_enum cmd = TX_OFF;
                    CmdPacket genesis_packet(get_cmd_info(cmd).cmd_str, arg1, arg2);
                    private_handle_cmd(cmd, genesis_packet);

                    clock_gettime(TIMER_CLOCK_ID, &current_time);
                    previous_time.tv_sec = current_time.tv_sec;
                    previous_time.tv_nsec = current_time.tv_nsec;
                }
            }
        }
    }; //end while
    LOG_ANNOYING("%s:%d Exiting\n",__FUNCTION__,__LINE__);
    return NULL;
}

CmdBase::tGenesisErr CmdBase::set_name(const char* name)
{
    CmdBase::tGenesisErr rtn = FAILED_TO_SEND;
    if (NULL != m_dev_handle)
    {
        CmdPacket::tp_constGenesisCmd cmd = get_cmd_info(SET_NAME).cmd_str;
        CmdPacket::tconstGenesisArg1 arg1 = "Genesis";

        CmdPacket packet(cmd, arg1);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(m_dev_handle, packet.GetPacket(), GENESIS_PACKET_LEN);
        if(GENESIS_PACKET_LEN == nBytes)
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

CmdBase::tGenesisErr CmdBase::private_set_freq(const long freq, const char* p_cmd)
{
    CmdBase::tGenesisErr rtn = FAILED_TO_SEND;
    if (NULL != m_dev_handle)
    {
        bool smooth = false;
        char arg1[GENESIS_ARG1_LENGTH];
        memset(arg1, 0, GENESIS_ARG1_LENGTH);

        char arg2[GENESIS_ARG2_LENGTH];
        memset(arg2, 0, GENESIS_ARG2_LENGTH);

        CmdPacket::tp_constGenesisCmd cmd = p_cmd;

        char freq_str[GENESIS_ARG1_LENGTH+1];
        memset(freq_str, 0, GENESIS_ARG1_LENGTH+1);
        snprintf(freq_str, GENESIS_ARG1_LENGTH+1, "%08ld", freq);
        for(int i=0; i< GENESIS_ARG1_LENGTH; i++)
        {
            arg1[i] = freq_str[i];
        }

        uint8_t regs[6];
        memset(regs, 0, sizeof(regs));
        uint32_t lo_freq = freq *4; //quadature mixer clock
        si570_set_frequency(lo_freq, regs);
        if (smooth)
        {
            cmd = get_cmd_info(SMOOTH).cmd_str;
        }

        arg2[0x2]=0xaa; // set i2c address in first "register"
        for(int i=0; i<6; i++)
        {
            arg2[i+0x4]=regs[i];
        }

        CmdPacket packet(cmd, arg1, arg2);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(m_dev_handle, packet.GetPacket(), GENESIS_PACKET_LEN);
        if(GENESIS_PACKET_LEN == nBytes)
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

CmdBase::tGenesisErr CmdBase::set_freq(const long freq)
{
    return private_set_freq(freq, get_cmd_info(SET_FREQ).cmd_str);
}

CmdBase::tGenesisErr CmdBase::smooth(const long freq)
{
    return private_set_freq(freq, get_cmd_info(SMOOTH).cmd_str);
}

CmdBase::tGenesisErr CmdBase::private_send_on_off_cmd(const bool on_off, const char *p_on_cmd, const char *p_off_cmd)
{
    CmdBase::tGenesisErr rtn = FAILED_TO_SEND;
    if (NULL != m_dev_handle)
    {
        CmdPacket::tp_constGenesisCmd cmd;
        if (on_off)
        {
            cmd = p_on_cmd;
        }
        else
        {
            cmd = p_off_cmd;
        }

        CmdPacket packet(cmd);
        packet.DumpPacket();
        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(m_dev_handle, packet.GetPacket(), GENESIS_PACKET_LEN);
        if(GENESIS_PACKET_LEN == nBytes)
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
CmdBase::tGenesisErr CmdBase::af_amp(const bool on_off)
{
    return private_send_on_off_cmd(on_off, get_cmd_info(AF_ON).cmd_str, get_cmd_info(AF_OFF).cmd_str);
}

CmdBase::tGenesisErr CmdBase::rf_preamp(const bool on_off)
{
    return private_send_on_off_cmd(on_off, get_cmd_info(RF_ON).cmd_str, get_cmd_info(RF_OFF).cmd_str);
}

CmdBase::tGenesisErr CmdBase::att(const bool on_off)
{
    return private_send_on_off_cmd(on_off, get_cmd_info(ATT_ON).cmd_str, get_cmd_info(ATT_OFF).cmd_str);
}

CmdBase::tGenesisErr CmdBase::mute(const bool on_off)
{
    return private_send_on_off_cmd(on_off, get_cmd_info(MUTE_ON).cmd_str, get_cmd_info(MUTE_OFF).cmd_str);
}

CmdBase::tGenesisErr CmdBase::trv(const bool on_off)
{
    return private_send_on_off_cmd(on_off, get_cmd_info(TRV_ON).cmd_str, get_cmd_info(TRV_OFF).cmd_str);
}

CmdBase::tGenesisErr CmdBase::private_cmd_arg2only(const unsigned char arg, const char *p_cmd)
{
    CmdBase::tGenesisErr rtn = FAILED_TO_SEND;
    if (NULL != m_dev_handle)
    {
        char arg1[GENESIS_ARG1_LENGTH];
        memset(arg1, 0, GENESIS_ARG1_LENGTH);
        char arg2[GENESIS_ARG2_LENGTH];
        memset(arg2, 0, GENESIS_ARG2_LENGTH);

        CmdPacket::tp_constGenesisCmd cmd = p_cmd;
        arg2[0x04] = arg;

        CmdPacket packet(cmd, arg1, arg2);
        packet.DumpPacket();

        #ifdef SIMULATE_USB_CONNECTION
        return OK;
        #endif
        int nBytes = write_to_device(m_dev_handle, packet.GetPacket(), GENESIS_PACKET_LEN);
        if(GENESIS_PACKET_LEN == nBytes)
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

CmdBase::tGenesisErr CmdBase::set_filt(const int fltr)
{
    return private_cmd_arg2only((fltr & 0xff), get_cmd_info(SET_FILT).cmd_str);
}

CmdBase::tGenesisErr CmdBase::tx(const bool on_off)
{
    tGenesisErr rtn = OK;
    if (on_off)
    {
        rtn = private_cmd_arg2only(0x03,get_cmd_info(TX_ON).cmd_str);
    }
    else
    {
        rtn = private_cmd_arg2only(0x00,get_cmd_info(TX_OFF).cmd_str);
    }
    return rtn;
}

CmdBase::tGenesisErr CmdBase::pa10(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff), get_cmd_info(PA10_ON).cmd_str);
}

CmdBase::tGenesisErr CmdBase::line_mic(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff), get_cmd_info(LINE_MIC).cmd_str);
}

CmdBase::tGenesisErr CmdBase::auto_cor(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff), get_cmd_info(AUTO_COR).cmd_str);
}

CmdBase::tGenesisErr CmdBase::sec_rx2(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff), get_cmd_info(SEC_RX2).cmd_str);
}

CmdBase::tGenesisErr CmdBase::monitor(const bool on_off)
{
    return private_cmd_arg2only(((on_off?0x01:0x00) & 0xff), get_cmd_info(MONITOR).cmd_str);
}

CmdBase::tGenesisErr CmdBase::k_speed(const int wpm)
{
    if ((7 > wpm)||(128 < wpm))
    {
        return BAD_ARG;
    }
    unsigned char divisor = (unsigned char)((520/wpm) & 0xff);
    return private_cmd_arg2only(divisor, get_cmd_info(K_SPEED).cmd_str);
}

CmdBase::tGenesisErr CmdBase::k_mode(const int mode)
{
    unsigned char mode_code = 0x01;
    switch (mode)
    {
    // Iambic
    case K_MODE_IAMBIC:
        mode_code = 0x02;
        break;

    // Iambic | Key-B 
    case K_MODE_IAMBIC | K_MODE_KEYER_MODE_B:
        mode_code = 0x05;
        break;

    // Iambic | Rev
    case K_MODE_IAMBIC | K_MODE_REV_PADDLE:
        mode_code = 0x03;
        break;

    // Iambic | Key-B | Rev
    case K_MODE_IAMBIC | K_MODE_KEYER_MODE_B | K_MODE_REV_PADDLE:
        mode_code = 0x06;
        break;

    // No Iambic
    case K_MODE_NONE:
    default:
        mode_code = 0x01;
        break;
    }

    return private_cmd_arg2only((unsigned char)(mode_code & 0xff), get_cmd_info(K_MODE).cmd_str);
}

CmdBase::tGenesisErr CmdBase::k_ratio(const double ratio)
{
    double tmp_ratio = ratio;
    if(10.0 < tmp_ratio) tmp_ratio = 10.0;
    if (1.0 > tmp_ratio) tmp_ratio = 1.0;
    unsigned int ratio_tenths = static_cast<int>((tmp_ratio - 1.0) * 10.0);
    unsigned int ratio_code = 10 + ratio_tenths;
    
    return private_cmd_arg2only((unsigned char)(ratio_code & 0xff), get_cmd_info(K_RATIO).cmd_str);
}

const CmdBase::t_cmd_enum CmdBase::private_str2cmd(std::string cmd)
{
    int i = 0;
    t_cmd_enum rtn = NONE;
    for(i = 0; LAST_CMD > i; i++)
    {
        if(cmd == get_cmd_info(static_cast<t_cmd_enum>(i)).cmd_str)
        {
            rtn = get_cmd_info(static_cast<t_cmd_enum>(i)).cmd;
            break;
        }
    }
    return rtn;
}

CmdBase::t_tx_state CmdBase::private_handle_cmd(t_cmd_enum cmd, CmdPacket &packet)
{
    t_tx_state tx_state = TX_STATE_IGNORE;

    if (NULL == mp_observer)
    {
        return tx_state;
    }

    switch(cmd)
    {
    case SET_NAME:
        {
            mp_observer->OnSetName();
        }
        break;

    case SET_FREQ:
        {
            mp_observer->OnSetFreq();
        }
        break;

    case SMOOTH:
        {
            mp_observer->OnSmooth();
        }
        break;

    case AF_ON:
        {
            mp_observer->OnAF(ON);
        }
        break;

    case AF_OFF:
        {
            mp_observer->OnAF(OFF);
        }
        break;

    case RF_ON:
        {
            mp_observer->OnRF(ON);
        }
        break;

    case RF_OFF:
        {
            mp_observer->OnRF(OFF);
        }
        break;

    case ATT_ON:
        {
            mp_observer->OnATT(ON);
        }
        break;

    case ATT_OFF:
        {
            mp_observer->OnATT(OFF);
        }
        break;

    case MUTE_ON:
        {
            mp_observer->OnMUTE(ON);
        }
        break;

    case MUTE_OFF:
        {
            mp_observer->OnMUTE(OFF);
        }
        break;

    case TRV_ON:
        {
            mp_observer->OnTRV(ON);
        }
        break;

    case TRV_OFF:
        {
            mp_observer->OnTRV(OFF);
        }
        break;

    case SET_FILT:
        {
            //todo
            mp_observer->OnSetFilt(0);
        }
        break;

    case TX_ON:
        {
            mp_observer->OnTX(ON);
            tx_state = TX_STATE_ON;
        }
        break;

    case TX_OFF:
        {
            mp_observer->OnTX(OFF);
            tx_state = TX_STATE_DROPPING;
        }
        break;

    case PA10_ON:
        {
            //todo
            mp_observer->OnPA10(ON);
        }
        break;

    case LINE_MIC:
        {
            //todo
            mp_observer->OnLineMic(ON);
        }
        break;
    case AUTO_COR:
        {
            //todo
            mp_observer->OnAutoCOR(ON);
        }
        break;

    case SEC_RX2:
        {
            //todo
            mp_observer->OnSecondRx(ON);
        }
        break;

    case MONITOR:
        {
            //todo
            mp_observer->OnMonitor(ON);
        }
        break;

    case K_SPEED:
        {
            //todo
            mp_observer->OnKeySpeed();
        }
        break;

    case K_MODE:
        {
            //todo
            mp_observer->OnKeyMode();
        }
        break;

    case K_RATIO:
        {
            //todo
            mp_observer->OnKeyRatio();
        }
        break;

    case DOT_ON:
        {
            mp_observer->OnDOT(ON);
            tx_state = TX_STATE_ON;
        }
        break;

    case DOT_OFF:
        {
            mp_observer->OnDOT(OFF);
            tx_state = TX_STATE_DROPPING;
        }
        break;

    case DASH_ON:
        {
            mp_observer->OnDASH(ON);
            tx_state = TX_STATE_ON;
        }
        break;

    case DASH_OFF:
        {
            mp_observer->OnDASH(OFF);
            tx_state = TX_STATE_DROPPING;
        }
        break;

    case IDLE:
        {
            mp_observer->OnIdle();
        }
        break;

    default:
        {
        }
        break;
    }

    return tx_state;
}

static std::string private_trim(std::string str)
{
  str.erase(str.begin(), std::find_if(str.begin(), str.end(),
    [](char& ch)->bool { return !std::isspace(ch); }));
  str.erase(std::find_if(str.rbegin(), str.rend(),
    [](char& ch)->bool { return !std::isspace(ch); }).base(), str.end());
  return str;
} 

static std::string private_trim_str(char* p_str)
{
    std::string tmp = p_str;
    return private_trim(tmp);
}

CmdBase::t_cmd_enum CmdBase::private_parse_packet(CmdPacket &packet)
{
    static t_cmd_enum last_cmd = NONE;
    char rx_cmd_str[GENESIS_COMMAND_LENGTH + 1] = {};

    rx_cmd_str[GENESIS_COMMAND_LENGTH] = '\0';
    packet.GetCmd(rx_cmd_str);
    t_cmd_enum cmd = private_str2cmd(private_trim_str(rx_cmd_str));
    return cmd;
}

void CmdBase::set_tx_dropout_ms(unsigned long tx_dropout_ms)
{
    m_tx_dropout_timeout_ns = tx_dropout_ms * NSEC_PER_MILLISEC;
}

void CmdBase::register_observer(Genesis_Observer *p_observer)
{
    LOG_INFO("%s:%d p_observer=%p\n",__FUNCTION__,__LINE__,p_observer);
    mp_observer = p_observer;
}
