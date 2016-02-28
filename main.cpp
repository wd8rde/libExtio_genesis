#include <unistd.h>
#include <stdio.h>
#include "extio.h"

int main(int argc, char** argv)
{
    char name[256];
    char model[256];
    int type = 0;
    InitHW(name, model, type);
    OpenHW();
    ShowGUI();
    return 0;
}
