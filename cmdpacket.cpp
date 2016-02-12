#include <stdio.h>
#include <string.h>
#include "cmdpacket.h"


#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);}

//#define DO_ANNOYING
#ifdef DO_ANNOYING
#define LOG_ANNOYING(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#else
#define LOG_ANNOYING(...) {}
#endif

CmdPacket::CmdPacket()
{
    memset( (m_packet), 0, GENESIS_PACKET_LEN);
}

CmdPacket::CmdPacket(tGenesisPacket packet)
{
    SetPacket(packet);
}

CmdPacket::CmdPacket(tp_constGenesisCmd cmd, tconstGenesisArg1 arg1, tconstGenesisArg2 arg2)
{
    memset( (m_packet), 0, GENESIS_PACKET_LEN);
    SetPacket(cmd,arg1,arg2);
}

void CmdPacket::SetPacket(tGenesisPacket packet)
{
    if (NULL != packet)
    {
        memcpy(m_packet, packet, GENESIS_PACKET_LEN);
    }
}

void CmdPacket::SetPacket(tp_constGenesisCmd cmd, tconstGenesisArg1 arg1, tconstGenesisArg2 arg2)
{
    SetCmd(cmd);
    SetArg1(arg1);
    SetArg2(arg2);
}

void CmdPacket::SetCmd(tp_constGenesisCmd cmd)
{
    if (NULL == cmd)
    {
        return;
    }
    //command are padded with spaces
    memset( (m_packet + GENESIS_COMMAND_OFFSET), 0x20, GENESIS_COMMAND_LENGTH);
    size_t length = strnlen(cmd, GENESIS_COMMAND_LENGTH);
    // do not include the terminating 0.
    memcpy( (m_packet + GENESIS_COMMAND_OFFSET), cmd, length);
}

void CmdPacket::SetArg1(tconstGenesisArg1 arg)
{
    if (NULL == arg)
    {
        return;
    }

    memset( (m_packet + GENESIS_ARG1_OFFSET), 0, GENESIS_ARG1_LENGTH);
    memcpy( (m_packet + GENESIS_ARG1_OFFSET), arg, GENESIS_ARG1_LENGTH);
}

void CmdPacket::SetArg2(tconstGenesisArg2 arg)
{
    if (NULL == arg)
    {
        return;
    }
    memset( (m_packet + GENESIS_ARG2_OFFSET), 0, GENESIS_ARG2_LENGTH);
    memcpy( (m_packet + GENESIS_ARG2_OFFSET), arg, GENESIS_ARG2_LENGTH);
}

CmdPacket::tGenesisPacket CmdPacket::GetPacket()
{
    return m_packet;
}

int CmdPacket::GetCmd(char cmd[])
{
    if (GENESIS_COMMAND_LENGTH > sizeof cmd)
    { 
        return -1;
    }
    memcpy(cmd, (m_packet + GENESIS_COMMAND_OFFSET), GENESIS_COMMAND_LENGTH);
    return 0;
}

int CmdPacket::GetArg1(char arg[])
{
    if (GENESIS_ARG1_LENGTH > sizeof arg)
    { 
        return -1;
    }
    memcpy(arg, (m_packet + GENESIS_ARG1_OFFSET), GENESIS_ARG1_LENGTH);
    return 0;
}

int CmdPacket::GetArg2(char arg[])
{
    if (GENESIS_ARG2_LENGTH > sizeof arg)
    { 
        return -1;
    }
    memcpy(arg, (m_packet + GENESIS_ARG2_OFFSET), GENESIS_ARG2_LENGTH);
    return 0;
}

#include <ctype.h>
void CmdPacket::DumpPacket()
{
    bool stop =false;
    char addr[1024];
    char buf[1024];
    char buf2[1024];
    char *a = &addr[0];
    char *p = &buf[0];
    char *q = &buf2[0];
    for(int i=0; i< GENESIS_PACKET_LEN; i++)
    {
        a += sprintf(a,"%02x ", i);
        p += sprintf(p,"%02x ", m_packet[i]);
        q += sprintf(q,"%c ", isprint?m_packet[i]:'.');
    }
    LOG_ANNOYING("0x%s\n",&addr[0]);
    LOG_ANNOYING("0x%s\n",&buf[0]);
    LOG_ANNOYING("  %s\n",&buf2[0]);
}

bool CmdPacket::operator==(const CmdPacket &other) const
{
    bool result = true;
    for(int i=0; i< GENESIS_PACKET_LEN; i++)
    {
        if(m_packet[i] != other.m_packet[i])
        {
            result = false;
            break;
        }
    }
    return result;
}

bool CmdPacket::operator!=(const CmdPacket &other) const 
{
    return !(*this == other);
}
