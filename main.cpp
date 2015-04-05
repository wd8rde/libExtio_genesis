#include <unistd.h>
#include <stdio.h>
#include "g59cmd.h"

int main(int argc, char** argv)
{
    G59Cmd g59;
    g59.Init(0xfffe,0x1970);
    g59.att(true);
    sleep(3);
    g59.att(false);

    return 0;
}
