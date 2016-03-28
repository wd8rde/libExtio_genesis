#include <string>
#include <iostream>
#include <fstream>

#include "genesis.h"
#include "cmdbase.h"
#include "g59cmd.h"
#include "g11cmd.h"
#include "simpleini-master/SimpleIni.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);}
#define LOG_INFO(...) //{fprintf(stderr,__VA_ARGS__);}
/**************************************************************/
/** Base Class Genesis
/**************************************************************/

#define SMOOTH_RANGE 3500 //Smooth tuning range for si570 in ppm

static const char* INIFILENAME = ".genesis-sdr.ini";
static const unsigned long TX_DROPOUT_MS = 300;

Genesis::Genesis(int productid)
    :m_vendorid(0xfffe)
    ,m_productid(productid)
    ,m_initialized(false)
    ,m_hasMicPreamp(true)
    ,m_hasGPA10(true)
    ,m_tx_dropout_ms(0)
    ,m_keyer_mode(CmdBase::K_MODE_NONE)
    ,m_keyer_speed(13)
    ,m_keyer_ratio(3.0)
    ,m_current_filter(0)
    ,m_current_freq(0)
    ,mp_cmd(NULL)
{
}

Genesis::~Genesis()
{
    if (m_initialized)
    {
        Close();
    }
}

void Genesis::register_observer(Genesis_Observer *p_observer)
{
    mp_cmd->register_observer(p_observer);
}

std::string Genesis::GetMake()
{ 
    return std::string("Genesis SDR"); 
}

int Genesis::GetVendorId()
{
    return m_vendorid;
}

bool Genesis::Init()
{
    if (!m_initialized)
    {
        m_initialized = mp_cmd->Init(m_vendorid, m_productid);
    }

    LoadConfigFile();
    bool hasmultiple = false;
    m_hasGPA10 = m_ini.GetBoolValue("g59","hasGPA10", true, &hasmultiple);
    m_ini.SetBoolValue("g59","hasGPA10",m_hasGPA10,"# true if PA10 enabled", true);
    m_hasMicPreamp = m_ini.GetBoolValue("g59","hasMicPreamp", true, &hasmultiple);
    m_ini.SetBoolValue("g59","hasMicPreamp",m_hasMicPreamp,"# true if Mic Preamp enabled", true);

    //enable the GPA10 if it is available
    mp_cmd->pa10(m_hasGPA10);

    //setup the keyer
    m_keyer_ratio = m_ini.GetDoubleValue("g59", "keyerRatio",3.0, &hasmultiple);
    m_ini.SetDoubleValue("g59","keyerRatio",m_keyer_ratio,"# keyer weight 1:ratio", true);
    m_keyer_speed = m_ini.GetLongValue("g59", "keyerSpeed",13, &hasmultiple);
    m_ini.SetLongValue("g59","keyerSpeed",m_keyer_speed,"# keyer speed in wpm", true);
    m_keyer_mode = m_ini.GetLongValue("g59", "keyerMode",CmdBase::K_MODE_NONE, &hasmultiple);
    m_ini.SetLongValue("g59","keyerMode",m_keyer_mode,"# keyer mode code", true);

    mp_cmd->k_ratio(m_keyer_ratio);
    mp_cmd->k_speed(m_keyer_speed);
    mp_cmd->k_mode(m_keyer_mode);

    m_tx_dropout_ms = m_ini.GetLongValue("g59", "txDropoutMS", TX_DROPOUT_MS, &hasmultiple);
    m_ini.SetLongValue("g59","txDropoutMS",m_tx_dropout_ms,"# TX Dropout time in milliseconds", true);
    mp_cmd->set_tx_dropout_ms(m_tx_dropout_ms);

    return m_initialized;
}

bool Genesis::Close()
{
    if (m_initialized)
    {
        SaveConfigFile();
        m_initialized = !mp_cmd->Close();
    }

    return m_initialized;
}

bool Genesis::SetLO(long freq)
{
    long smooth_amount = (m_current_freq * SMOOTH_RANGE)/1000000;
    if(smooth_amount >= (abs(freq - m_current_freq)))
    {
        mp_cmd->smooth(freq);
    }
    else
    {
        mp_cmd->set_freq(freq);
        m_current_freq = freq;
    }
}

bool Genesis::SetBand(long freq)
{
    int band_filter = FindBand(freq);

    if(m_current_filter != band_filter )
    {
        mp_cmd->set_filt(band_filter);
        m_current_filter = band_filter;
    }
}

int Genesis::FindBand(long freq)
{
    int index = 0;
    for(BandFilters_t::const_iterator it = m_bandfilters.begin(); it != m_bandfilters.end(); it++ )
    {
        if((it->low_freq <= freq) && (it->high_freq >= freq))
        {
            index = it->index;
            //LOG_INFO("%s:%d Found Band: Found %d, freq %ld, low %ld, high %ld\n",__FUNCTION__,__LINE__,index, freq, it->low_freq, it->high_freq);
            break;
        }
    }
    return index;
}

bool Genesis::SetTx(bool tx_enable)
{
    if (tx_enable)
    {
        //enable the Mic Preamp if it is available
        if (m_hasMicPreamp)
        {
            mp_cmd->line_mic(true);
        }

        //Start Transmitting
        mp_cmd->tx(true);
        //Todo, there should be a tx time limit timer
    }
    else
    {
        //Stop Transmitting
        mp_cmd->tx(false);
        //disable the Mic Preamp
        mp_cmd->line_mic(false);
    }

    return true;
}
void Genesis::SetAtten(bool on)
{
    mp_cmd->att(on);
}
void Genesis::SetRFPreamp(bool on)
{
    mp_cmd->rf_preamp(on);
}

void Genesis::EnableLineMic(bool onoff)
{
    if(onoff)
    {
        m_hasMicPreamp = true;
        m_ini.SetBoolValue("g59","hasMicPreamp",m_hasMicPreamp,"# true if Mic Preamp enabled", true);
    }
    else
    {
        m_hasMicPreamp = true;
        m_ini.SetBoolValue("g59","hasMicPreamp",m_hasMicPreamp,"# true if Mic Preamp enabled", true);
    }
}

void Genesis::EnablePA10(bool onoff)
{
    if(onoff)
    {
        m_hasGPA10 = true;
        m_ini.SetBoolValue("g59","hasGPA10",m_hasGPA10,"# true if PA10 enabled", true);
    }
    else
    {
        m_hasGPA10 = false;
        m_ini.SetBoolValue("g59","hasGPA10",m_hasGPA10,"# true if PA10 enabled", true);
    }
    if(NULL != mp_cmd)
    {
        mp_cmd->pa10(m_hasGPA10);
    }
}
void Genesis::SetWpm(int wpm)
{
    m_keyer_speed = wpm;
    m_ini.SetLongValue("g59","keyerSpeed",m_keyer_speed,"# keyer speed in wpm", true);
    if(NULL != mp_cmd)
    {
        mp_cmd->k_speed(wpm);
    }
}

void Genesis::SetKeyerRatio(float ratio_dot_to_dash)
{
    m_keyer_ratio = ratio_dot_to_dash;
    m_ini.SetDoubleValue("g59","keyerRatio",m_keyer_ratio,"# keyer weight 1:ratio", true);
    if(NULL != mp_cmd)
    {
        mp_cmd->k_ratio(m_keyer_ratio);
    }
}

void Genesis::SetKeyerMode(int keyer_mode)
{
    m_keyer_mode = keyer_mode;
    m_ini.SetLongValue("g59","keyerMode",m_keyer_mode,"# keyer mode code", true);
    if(NULL != mp_cmd)
    {
        mp_cmd->k_mode(m_keyer_mode);
    }
}

bool Genesis::LoadConfigFile()
{
    bool rtn = false;
    m_ini.SetUnicode(true);
    m_ini.SetMultiKey(false);
    m_ini.SetMultiLine(false);

    // load the file
    std::string homepath = getenv("HOME");
    std::string inifilepath((homepath + "/" + INIFILENAME).c_str());
    std::ifstream instream;
    instream.open(inifilepath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (instream.is_open())
    {
        if (m_ini.LoadData(instream) < 0)
        {
            LOG_ERR("%s:%d Failed to load config information from %s\n",__FUNCTION__,__LINE__,inifilepath.c_str());
            return false;
        }
        rtn = true;
        instream.close();
    }
    else
    {
        LOG_ERR("%s:%d Failed to open %s for reading\n",__FUNCTION__,__LINE__,inifilepath.c_str());
    }
    return rtn;
}

bool Genesis::SaveConfigFile()
{
    bool rtn = false;
    m_ini.SetUnicode(true);
    m_ini.SetMultiKey(false);
    m_ini.SetMultiLine(false);

    // load the file
    std::string homepath = getenv("HOME");
    std::string inifilepath((homepath + "/" + INIFILENAME).c_str());
    std::ofstream outstream;
    outstream.open(inifilepath.c_str(), std::ofstream::out | std::ofstream::binary  | std::ofstream::trunc);
    if (outstream.is_open())
    {
        if (m_ini.Save(outstream) < 0)
        {
            LOG_ERR("%s:%d Failed to save config information from %s\n",__FUNCTION__,__LINE__,inifilepath.c_str());
            return false;
        }
        rtn = true;
        outstream.close();
    }
    else
    {
        LOG_ERR("%s:%d Failed to open %s for writing\n",__FUNCTION__,__LINE__,inifilepath.c_str());
    }
    return rtn;
}

/**************************************************************/
/** Class G59                                                 */
/**************************************************************/
const Genesis::BandFilters_t G59::ms_g59_bandfilters(
{
    { 
        1,
        "160m",
        1800000,
        2000000
    },

    { 
        2,
        "80m",
        3500000,
        4000000
    },
    { 
        3,
        "60-40m",
        5403500,
        7300000
    },
    { 
        4,
        "30-20m",
        10100000,
        14350000
    },
    { 
        5,
        "17-15m",
        18068000,
        21450000
    },
    { 
        6,
        "12-10m",
        24890000,
        29700000
    },
    { 
        7,
        "6m",
        50000000,
        54000000
    },
    { 
        0,
        "gen",
        0,
        1024000000
    }
});

G59::G59() 
    : Genesis(0x1970)
{
    mp_cmd = new G59Cmd();
    m_bandfilters = ms_g59_bandfilters;
};

G59::~G59()
{
    if (NULL != mp_cmd)
    {
        delete mp_cmd;
        mp_cmd = NULL;
    }
};

int G59::GetProductId()
{
    return m_productid;
};

std::string G59::GetModel()
{
    return std::string("G59");
}

/**************************************************************/
/** Class G11                                                 */
/**************************************************************/
const Genesis::BandFilters_t G11::ms_g11_bandfilters(
{
    {
        0,
        "160m",
        1800000,
        2000000
    },

    {
        1,
        "80m",
        3500000,
        4000000
    },
    {
        2,
        "60-40m",
        5403500,
        7300000
    },
    {
        3,
        "30m",
        10100000,
        10150000
    },
    {
        4,
        "20-17m",
        14000000,
        18168000
    },
    {
        5,
        "gen",
        0,
        1024000000
    }
});

G11::G11() 
    : Genesis(0x1971)
{
    mp_cmd = new G11Cmd();
    m_bandfilters = ms_g11_bandfilters;
};

G11::~G11()
{
    if (NULL != mp_cmd)
    {
        delete mp_cmd;
        mp_cmd = NULL;
    }
};

int G11::GetProductId()
{
    return m_productid;
};

std::string G11::GetModel()
{
    return std::string("G11");
}

