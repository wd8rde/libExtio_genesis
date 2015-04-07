#include "genesis.h"
#include <string>
#include "g59cmd.h"
/**************************************************************/
/** Base Class Genesis
/**************************************************************/

const Genesis::BandFilters_t Genesis::ms_bandfilters(
{
    { 
        .index = 1,
        .band_str = "160m",
        .low_freq = 1800000,
        .high_freq = 2000000
    },

    { 
        .index = 2,
        .band_str = "80m",
        .low_freq = 3500000,
        .high_freq = 4000000
    },
    { 
        .index = 3,
        .band_str = "60-40m",
        .low_freq = 5403500,
        .high_freq = 7000000
    },
    { 
        .index = 4,
        .band_str = "30-20m",
        .low_freq = 10100000,
        .high_freq = 14350000
    },
    { 
        .index = 5,
        .band_str = "17-15m",
        .low_freq = 18068000,
        .high_freq = 21450000
    },
    { 
        .index = 6,
        .band_str = "12-10m",
        .low_freq = 24890000,
        .high_freq = 29700000
    },
    { 
        .index = 7,
        .band_str = "6m",
        .low_freq = 50000000,
        .high_freq = 54000000
    },
    { 
        .index = 0,
        .band_str = "gen",
        .low_freq = 0,
        .high_freq = 1024000000
    }
});

Genesis::Genesis(int productid)
    :m_vendorid(0xfffe)
    ,m_productid(productid)
    ,m_initialized(false)
{
}

Genesis::~Genesis()
{
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
        m_initialized = m_g59cmd.Init(m_vendorid, m_productid);
    }

    return m_initialized;
}

bool Genesis::Close()
{
    if (m_initialized)
    {
        m_initialized = !m_g59cmd.Close();
    }

    return m_initialized;
}

bool Genesis::SetLO(long freq)
{
    int band_filter = FindBand(freq);
    m_g59cmd.set_filt(band_filter);
    m_g59cmd.set_freq(freq);
}

int Genesis::FindBand(long freq)
{
    int index = 0;
    for(BandFilters_t::const_iterator it = ms_bandfilters.begin(); it != ms_bandfilters.end(); it++ )
    {
        if((it->low_freq <= freq) && (it->high_freq >= freq))
        {
            index = it->index;
            fprintf(stderr, "%s:%d Found Band: %d\n",__FUNCTION__,__LINE__,index);
            break;
        }
    }
    return index;
}

/**************************************************************/
/** Class G59                                                 */
/**************************************************************/
G59::G59() 
    : Genesis(0x1970)
{
};

G59::~G59()
{
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
G11::G11() 
    : Genesis(0x1971)
{
};

G11::~G11()
{
};

int G11::GetProductId()
{
    return m_productid;
};

std::string G11::GetModel()
{
    return std::string("G11");
}

