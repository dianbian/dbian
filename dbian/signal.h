#pragma once 

#include "unp.h"

Sigfunc *
Signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;  //BSD
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;
    return oact.sa_handler;
}

void    //wait child process release fd
Sig_child(int sigo)
{
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) >0 )
        printf("child %d terminated \n", pid);
    return;   
}