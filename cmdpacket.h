#ifndef CMDPACKET_H_
#define CMDPACKET_H_

#include "hid_util.h"

#define GENESIS_COMMAND_OFFSET 0x00
#define GENESIS_COMMAND_LENGTH 8
#define GENESIS_ARG1_OFFSET 0x08
#define GENESIS_ARG1_LENGTH 8
#define GENESIS_ARG2_OFFSET 0x10
#define GENESIS_ARG2_LENGTH 48
#define GENESIS_PACKET_LEN 64

class CmdPacket
{
public:
    typedef char tGenesisCmd[GENESIS_COMMAND_LENGTH];
    typedef char* tp_GenesisCmd;
    typedef const char* tp_constGenesisCmd;
    typedef const char tconstGenesisCmd[GENESIS_COMMAND_LENGTH];
    typedef char tGenesisArg1[GENESIS_ARG1_LENGTH];
    typedef const char tconstGenesisArg1[GENESIS_ARG1_LENGTH];
    typedef char tGenesisArg2[GENESIS_ARG2_LENGTH];
    typedef const char tconstGenesisArg2[GENESIS_ARG2_LENGTH];
    typedef unsigned char *tGenesisPacket;

    CmdPacket();
    CmdPacket(tGenesisPacket packet);
    CmdPacket(tp_constGenesisCmd cmd, tconstGenesisArg1 arg1 = NULL, tconstGenesisArg2 arg2 = NULL);
    virtual ~CmdPacket() {};

    void SetPacket(tGenesisPacket packet);
    void SetPacket(tp_constGenesisCmd cmd, tconstGenesisArg1 arg1 = NULL, tconstGenesisArg2 arg2 = NULL);
    int GetCmd(char cmd[]);
    int GetArg1(char arg[]);
    int GetArg2(char arg[]);
    void SetCmd(tp_constGenesisCmd cmd);
    void SetArg1(tconstGenesisArg1 arg);
    void SetArg2(tconstGenesisArg1 arg);
    tGenesisPacket GetPacket();
    void DumpPacket();
    bool operator==(const CmdPacket &other) const;
    bool operator!=(const CmdPacket &other) const;

protected:
    unsigned char m_packet[GENESIS_PACKET_LEN];
};
#endif /* CMDPACKET_H_*/
