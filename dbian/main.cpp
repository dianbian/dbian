#include "ser_main.h"
#include "comm/linkedList.h"
#include "comm/thread.h"

void * thread_work(void*)
{
    for ( int i = 0; i < 20; i++)
    {
        printf("the thread output the number:%d\n", i);
        sleep(1);
    }
    return nullptr;
}

int main(int argc, char** argv)
{
    //ser_main(argc, argv);
    //ser_main_fork(argc, argv);
    //ser_main_udp(argc, argv);
    //ser_main_poll(argc, argv);
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

    thread thread("main");
    thread.setRouter(thread_work);
    thread.run();
    sleep(50);

    thread.detach();
}