#include <stdio.h>
#include <string.h>
#include "g59cmdpacket.h"

#define G59_COMMAND_OFFSET 0x00
#define G59_COMMAND_LENGTH 8
#define G59_ARG1_OFFSET 0x08
#define G59_ARG1_LENGTH 8
#define G59_ARG2_OFFSET 0x10
#define G59_ARG2_LENGTH 8
#define G59_PACKET_LEN 64

G59CmdPacket::G59CmdPacket()
{
    memset( (m_packet), 0, G59_PACKET_LEN);
}

G59CmdPacket::G59CmdPacket(tconstG59Cmd cmd, tconstG59Arg1 arg1, tconstG59Arg2 arg2)
{
    memset( (m_packet), 0, G59_PACKET_LEN);
    SetPacket(cmd,arg1,arg2);
}

void G59CmdPacket::SetPacket(tconstG59Cmd cmd, tconstG59Arg1 arg1, tconstG59Arg2 arg2)
{
    SetCmd(cmd);
    SetArg1(arg1);
    SetArg2(arg2);
}

void G59CmdPacket::SetCmd(tconstG59Cmd cmd)
{
    if (NULL == cmd)
    {
        return;
    }
    //command are padded with spaces
    memset( (m_packet + G59_COMMAND_OFFSET), 0x20, G59_COMMAND_LENGTH);
    size_t length = strnlen(cmd, G59_COMMAND_LENGTH);
    memcpy( (m_packet + G59_COMMAND_OFFSET), cmd, length);
}

void G59CmdPacket::SetArg1(tconstG59Arg1 arg)
{
    if (NULL == arg)
    {
        return;
    }

    memset( (m_packet + G59_ARG1_OFFSET), 0, G59_ARG1_LENGTH);
    memcpy( (m_packet + G59_ARG1_OFFSET), arg, G59_ARG1_LENGTH);
}

void G59CmdPacket::SetArg2(tconstG59Arg2 arg)
{
    if (NULL == arg)
    {
        return;
    }
    memset( (m_packet + G59_ARG2_OFFSET), 0, G59_ARG2_LENGTH);
    memcpy( (m_packet + G59_ARG2_OFFSET), arg, G59_ARG2_LENGTH);
}

G59CmdPacket::tG59Packet G59CmdPacket::GetPacket()
{
    return m_packet;
}

void G59CmdPacket::DumpPacket()
{
    bool stop =false;
    char buf[1024];
    char *p = &buf[0];
    for(int i=0; i< G59_PACKET_LEN; i++)
    {
        p += sprintf(p,"%02x ", m_packet[i]);
    }
    fprintf(stderr,"0x%s\n",&buf[0]);
}

