#ifndef G59CMDPACKET_H_
#define G59CMDPACKET_H_

#include "hid_util.h"

#define G59_COMMAND_OFFSET 0x00
#define G59_COMMAND_LENGTH 8
#define G59_ARG1_OFFSET 0x08
#define G59_ARG1_LENGTH 8
#define G59_ARG2_OFFSET 0x10
#define G59_ARG2_LENGTH 48
#define G59_PACKET_LEN 64

class G59CmdPacket
{
public:
    typedef char tG59Cmd[G59_COMMAND_LENGTH];
    typedef char* tp_G59Cmd;
    typedef const char* tp_constG59Cmd;
    typedef const char tconstG59Cmd[G59_COMMAND_LENGTH];
    typedef char tG59Arg1[G59_ARG1_LENGTH];
    typedef const char tconstG59Arg1[G59_ARG1_LENGTH];
    typedef char tG59Arg2[G59_ARG2_LENGTH];
    typedef const char tconstG59Arg2[G59_ARG2_LENGTH];
    typedef unsigned char *tG59Packet;

    G59CmdPacket();
    G59CmdPacket(tG59Packet packet);
    G59CmdPacket(tp_constG59Cmd cmd, tconstG59Arg1 arg1 = NULL, tconstG59Arg2 arg2 = NULL);
    virtual ~G59CmdPacket() {};

    void SetPacket(tG59Packet packet);
    void SetPacket(tp_constG59Cmd cmd, tconstG59Arg1 arg1 = NULL, tconstG59Arg2 arg2 = NULL);
    int GetCmd(char cmd[]);
    int GetArg1(char arg[]);
    int GetArg2(char arg[]);
    void SetCmd(tp_constG59Cmd cmd);
    void SetArg1(tconstG59Arg1 arg);
    void SetArg2(tconstG59Arg1 arg);
    tG59Packet GetPacket();
    void DumpPacket();
    bool operator==(const G59CmdPacket &other) const;
    bool operator!=(const G59CmdPacket &other) const;

protected:
    unsigned char m_packet[G59_PACKET_LEN];
};
#endif /* G59CMDPACKET_H_*/
