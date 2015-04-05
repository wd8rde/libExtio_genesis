#ifndef GENESIS_SDR_H_
#define GENESIS_SDR_H_
#include <string>
#include <vector>
#include "g59cmd.h"

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
    virtual bool Init() = 0;
    virtual bool Close() = 0;
    virtual bool SetLO(long freq) = 0;
    int m_vendorid;
    int m_productid;
    bool m_initialized;
};

class G59: public Genesis
{
public:
    G59();
    virtual ~G59();
    virtual int GetProductId();
    virtual std::string GetModel();
    virtual bool Init();
    virtual bool Close();
    virtual bool SetLO(long freq);
protected:
    G59Cmd m_g59cmd;
    static const BandFilters_t ms_bandfilters;

    int FindBand(long freq);
};
#endif /*GENESIS_SDR_H_*/
