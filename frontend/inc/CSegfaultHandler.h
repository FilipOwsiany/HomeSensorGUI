#ifndef C_SEGFAULT_HANDLER_H
#define C_SEGFAULT_HANDLER_H

#include <signal.h>

class CSegfaultHandler
{
public:
    static void install();
};

#endif