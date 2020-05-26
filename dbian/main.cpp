#include "ser_main.h"
#include "comm/linkedList.h"
#include "comm/thread.h"
#include "comm/unp.h"
#define xxxxx 0

int main(int argc, char** argv)
{
    //ser_main(argc, argv);
    //ser_main_fork(argc, argv);
    //ser_main_udp(argc, argv);
    //ser_main_poll(argc, argv);

    epollFunc();
    
    while(1)
    {}
}