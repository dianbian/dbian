#include "ser_main.h"
#include "comm/linkedList.h"
#include "comm/thread.h"
#include "comm/unp.h"
#include "epoll.h"
#define xxxxx 0

int main(int argc, char** argv)
{
    //ser_main(argc, argv);
    //ser_main_fork(argc, argv);
    //ser_main_udp(argc, argv);
    //ser_main_poll(argc, argv);

    log *t = log::getInstance();
    t->initialize("bian.log");

    thread the("logThread");
    the.setRouter(log::runTask, t);
    the.run();

    epoll *ep = new epoll();
    if (!ep->initialize())
        return -1;

    thread emsg("emsg");
    emsg.setRouter(epoll::dealMsg, ep);
    emsg.run();

    thread wmsg("wmsg");
    wmsg.setRouter(epoll::dealSend, ep);
    wmsg.run();

    ep->loop();

}