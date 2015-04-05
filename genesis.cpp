#include "genesis.h"
#include <string>
#include "g59cmd.h"

Genesis::Genesis(int productid)
    :m_vendorid(0xfffe)
    ,m_productid(productid)
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

G59::G59() : Genesis(0x1970)
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

bool G59::Init()
{
    return m_g59cmd.Init(m_vendorid, m_productid);
}

