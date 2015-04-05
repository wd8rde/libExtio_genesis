#ifndef G59CMDPACKET_H_
#define G59CMDPACKET_H_

#include "usbutil.h"

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
    typedef char* tG59Cmd;
    typedef const char* tconstG59Cmd;
    typedef char tG59Arg1[G59_ARG1_LENGTH];
    typedef const char tconstG59Arg1[G59_ARG1_LENGTH];
    typedef char tG59Arg2[G59_ARG2_LENGTH];
    typedef const char tconstG59Arg2[G59_ARG2_LENGTH];
    typedef unsigned char *tG59Packet;

    G59CmdPacket();
    virtual ~G59CmdPacket() {};
    G59CmdPacket(tconstG59Cmd cmd, tconstG59Arg1 arg1 = NULL, tconstG59Arg2 arg2 = NULL);
    void SetPacket(tconstG59Cmd cmd, tconstG59Arg1 arg1 = NULL, tconstG59Arg2 arg2 = NULL);
    void SetCmd(tconstG59Cmd cmd);
    void SetArg1(tconstG59Arg1 arg);
    void SetArg2(tconstG59Arg1 arg);
    tG59Packet GetPacket();
    void DumpPacket();

protected:
    unsigned char m_packet[G59_PACKET_LEN];
};
#endif /* G59CMDPACKET_H_*/
