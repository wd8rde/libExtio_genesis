#ifndef GENESIS_SDR_H_
#define GENESIS_SDR_H_
#include <string>
#include "g59cmd.h"

class Genesis
{
public:
    Genesis(int productid);
    virtual ~Genesis();
    std::string GetMake();
    int GetVendorId();
    virtual std::string GetModel() = 0;
    virtual int GetProductId() = 0;
    virtual bool Init() = 0;
    int m_vendorid;
    int m_productid;
};

class G59: public Genesis
{
public:
    G59();
    virtual ~G59();
    virtual int GetProductId();
    virtual std::string GetModel();
    virtual bool Init();
protected:
    G59Cmd m_g59cmd;
};
#endif /*GENESIS_SDR_H_*/
