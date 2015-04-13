#ifndef GENESIS_SDR_H_
#define GENESIS_SDR_H_
#include <string>
#include <vector>
#include "g59cmd.h"
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
    bool LoadConfigFile();
    bool SaveConfigFile();
    int m_vendorid;
    int m_productid;
    bool m_initialized;
    G59Cmd m_g59cmd;
    static const BandFilters_t ms_bandfilters;
    bool m_hasMicPreamp;
    bool m_hasGPA10;
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
