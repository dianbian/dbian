#include "ser_main.h"
#include "comm/linkedList.h"
#include "comm/thread.h"

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
    writeFunc();
    
    sleep(100);
    /* int m_fd = ::open("bian", O_CREAT|O_RDWR, 0644);
     int len = 10;
     ftruncate(m_fd, len);
    char *addr = (char *)mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0); 
    if (addr == MAP_FAILED) {
            fprintf(stderr, "mmap() failed: %s\n", strerror(errno));
            exit(1);
        }
        memcpy(addr, "xxxx123456", 10);*/
}