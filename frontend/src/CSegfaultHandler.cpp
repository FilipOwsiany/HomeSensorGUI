#include "CSegfaultHandler.h"

#include <unistd.h>
#include <syscall.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace
{
    struct sigaction gOldSegvSig;
    struct sigaction gNewSegvSig;

    constexpr int BACKTRACE_DEPTH = 64;

    void SegvSignalHandler(int signal, siginfo_t *sigInfo, void *context)
    {
        sigset_t s;
        sigemptyset(&s);
        sigaddset(&s, signal);
        sigprocmask(SIG_UNBLOCK, &s, nullptr);

        pid_t pid = syscall(SYS_gettid);
        (void)pid;

        void *backTrace[BACKTRACE_DEPTH] = {0};
        int backtraceSize = backtrace(backTrace, BACKTRACE_DEPTH);
        char **backTraceSymbols = backtrace_symbols(backTrace, backtraceSize);

        printf("### SEGFAULT: stack trace:\n");
        for (int i = 0; i < backtraceSize; i++) 
        {
            Dl_info info;
            if (dladdr(backTrace[i], &info) && info.dli_sname) 
            {
                printf("(%d.) %s : %s\n", i, info.dli_fname, info.dli_sname);
            } 
            else 
            {
                printf("(%d.) %p\n", i, backTrace[i]);
            }
        }
        printf("### end of stack trace ###\n");

        free(backTraceSymbols);

        sigaction(SIGSEGV, &gOldSegvSig, nullptr);
        _exit(1);
    }
}

void CSegfaultHandler::install()
{
    memset(&gOldSegvSig, 0, sizeof(gOldSegvSig));
    memset(&gNewSegvSig, 0, sizeof(gNewSegvSig));

    gNewSegvSig.sa_sigaction = SegvSignalHandler;
    gNewSegvSig.sa_flags = SA_SIGINFO;

    if (sigaction(SIGSEGV, &gNewSegvSig, &gOldSegvSig) == -1) {
        printf("::sigaction SIGSEGV set new handler failed!\n");
    }
}
