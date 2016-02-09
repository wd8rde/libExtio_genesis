#ifndef GENESIS_SDR_H_
#define GENESIS_SDR_H_
#include <string>
#include <vector>
#include "cmdbase.h"
#include "simpleini-master/SimpleIni.h"

class Genesis
{
public:
    typedef struct _BandFilter_t
    {
        int index;
        std::string band_str;
        long low_freq;
        long high_freq;
    }BandFilter_t;

    typedef std::vector<BandFilter_t> BandFilters_t;

    Genesis(int productid);
    virtual ~Genesis();
    void register_observer(Genesis_Observer *p_observer);
    std::string GetMake();
    int GetVendorId();
    virtual std::string GetModel() = 0;
    virtual int GetProductId() = 0;
    virtual bool Init();
    virtual bool Close();
    virtual bool SetLO(long freq);
    virtual bool SetBand(long freq);
    int FindBand(long freq);
    bool SetTx(bool tx_enable);
    void SetAtten(bool on);
    void SetRFPreamp(bool on);
    void SetWpm(int wpm);

    bool LoadConfigFile();
    bool SaveConfigFile();
    int m_vendorid;
    int m_productid;
    bool m_initialized;
    CmdBase m_g59cmd;
    static const BandFilters_t ms_bandfilters;
    bool m_hasMicPreamp;
    bool m_hasGPA10;
    unsigned long m_tx_dropout_ms;
    int m_keyer_mode;
    int m_keyer_speed;
    double m_keyer_ratio;
    int m_current_filter;
    long m_current_freq;
    CSimpleIniA m_ini;


};

class G59: public Genesis
{
public:
    G59();
    virtual ~G59();
    virtual int GetProductId();
    virtual std::string GetModel();
};

class G11: public Genesis
{
public:
    G11();
    virtual ~G11();
    virtual int GetProductId();
    virtual std::string GetModel();
};
#endif /*GENESIS_SDR_H_*/
