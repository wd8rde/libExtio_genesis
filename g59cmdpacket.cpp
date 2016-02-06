#include <stdio.h>
#include <string.h>
#include "g59cmdpacket.h"

#if 0
#define G59_COMMAND_OFFSET 0x00
#define G59_COMMAND_LENGTH 8
#define G59_ARG1_OFFSET 0x08
#define G59_ARG1_LENGTH 8
#define G59_ARG2_OFFSET 0x10
#define G59_ARG2_LENGTH 48
#define G59_PACKET_LEN 64
#endif
#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);}

//#define DO_ANNOYING
#ifdef DO_ANNOYING
#define LOG_ANNOYING(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#else
#define LOG_ANNOYING(...) {}
#endif

G59CmdPacket::G59CmdPacket()
{
    memset( (m_packet), 0, G59_PACKET_LEN);
}

G59CmdPacket::G59CmdPacket(tG59Packet packet)
{
    SetPacket(packet);
}

G59CmdPacket::G59CmdPacket(tp_constG59Cmd cmd, tconstG59Arg1 arg1, tconstG59Arg2 arg2)
{
    memset( (m_packet), 0, G59_PACKET_LEN);
    SetPacket(cmd,arg1,arg2);
}

void G59CmdPacket::SetPacket(tG59Packet packet)
{
    if (NULL != packet)
    {
        memcpy(m_packet, packet, G59_PACKET_LEN);
    }
}

void G59CmdPacket::SetPacket(tp_constG59Cmd cmd, tconstG59Arg1 arg1, tconstG59Arg2 arg2)
{
    SetCmd(cmd);
    SetArg1(arg1);
    SetArg2(arg2);
}

void G59CmdPacket::SetCmd(tp_constG59Cmd cmd)
{
    if (NULL == cmd)
    {
        return;
    }
    //command are padded with spaces
    memset( (m_packet + G59_COMMAND_OFFSET), 0x20, G59_COMMAND_LENGTH);
    size_t length = strnlen(cmd, G59_COMMAND_LENGTH);
    // do not include the terminating 0.
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

int G59CmdPacket::GetCmd(char cmd[])
{
    if (G59_COMMAND_LENGTH > sizeof cmd)
    { 
        return -1;
    }
    memcpy(cmd, (m_packet + G59_COMMAND_OFFSET), G59_COMMAND_LENGTH);
    return 0;
}

int G59CmdPacket::GetArg1(char arg[])
{
    if (G59_ARG1_LENGTH > sizeof arg)
    { 
        return -1;
    }
    memcpy(arg, (m_packet + G59_ARG1_OFFSET), G59_ARG1_LENGTH);
    return 0;
}

int G59CmdPacket::GetArg2(char arg[])
{
    if (G59_ARG2_LENGTH > sizeof arg)
    { 
        return -1;
    }
    memcpy(arg, (m_packet + G59_ARG2_OFFSET), G59_ARG2_LENGTH);
    return 0;
}

#include <ctype.h>
void G59CmdPacket::DumpPacket()
{
    bool stop =false;
    char addr[1024];
    char buf[1024];
    char buf2[1024];
    char *a = &addr[0];
    char *p = &buf[0];
    char *q = &buf2[0];
    for(int i=0; i< G59_PACKET_LEN; i++)
    {
        a += sprintf(a,"%02x ", i);
        p += sprintf(p,"%02x ", m_packet[i]);
        q += sprintf(q,"%c ", isprint?m_packet[i]:'.');
    }
    LOG_ANNOYING("0x%s\n",&addr[0]);
    LOG_ANNOYING("0x%s\n",&buf[0]);
    LOG_ANNOYING("  %s\n",&buf2[0]);
}

bool G59CmdPacket::operator==(const G59CmdPacket &other) const
{
    bool result = true;
    for(int i=0; i< G59_PACKET_LEN; i++)
    {
        if(m_packet[i] != other.m_packet[i])
        {
            result = false;
            break;
        }
    }
    return result;
}

bool G59CmdPacket::operator!=(const G59CmdPacket &other) const 
{
    return !(*this == other);
}
