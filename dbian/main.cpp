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

#ifdef xxxxx
    LinkedList ll;
    ll.initList();
    ll.insertList("bianbian");
    ll.insertList("孙悟空");
    ll.insertList("唐僧");
    ll.insertList("猪八戒");
    ll.insertList("玉皇大帝");
    ll.insertList("如来佛祖");
    ll.insertList("bianbian");
    ll.printList();
    printf("ylllllllllllll\n");
    writeFunc();
    
    sleep(100);
#else
    LOG_DEBUG("%d, %f, %s", inumber, fnumber, string);
#endif
}