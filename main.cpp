#include <unistd.h>
#include <stdio.h>
#include "si570.h"

int main(int argc, char** argv)
{
    uint8_t regs[6];
    uint32_t freq = 40000000;
    si570_set_frequency( freq, regs );
    for(int i=0; i < 6; i++)
    {
        printf("reg[%d] = 0x%02x\n",i,regs[i]);
    }

    return 0;
}
