#pragma once
#include "comm/unp.h"
#include "comm/thread.h"
#include "socket.h"
#include "signal.h"
#include "communicate.h"
#include "epoll.h"

int
ser_main(int argc, char** argv)
{
    int listenfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(53101);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for ( ; ; ) {
        len = sizeof(cliaddr);
        printf("accept\n");
        connfd = Accept(listenfd, (SA *)&cliaddr, &len);
        printf("connection from %s, port %d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
            ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Writen(connfd, buff, strlen(buff));

        Close(connfd);
        sleep(1);
    }
    return 0;
}

int
ser_main_fork(int argc, char** argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    char buff[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(53101);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    Signal(SIGCHLD, Sig_child);

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        if ( (connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0) {
            if (errno == EINTR)
                continue;
            else
                exit(0);
        }
        printf("connection from %s, port %d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
            ntohs(cliaddr.sin_port));
        if ( (childpid = fork()) == 0) {    //child process
            Close(listenfd);    //close listening socket, child process not need
            str_echo(connfd);   //process the request
            exit(0);
        }
        Close(connfd);  //fd is duplicated, 
    }
    return 0;
}

int
ser_main_udp(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(53101);

    Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));

    dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
    return 0;
}

int
ser_main_poll(int argc, char** argv)
{
#define OPEN_MAX 1024   //clib del OPEN_MAX, use getrlimit
#define INFTIM -1   //not find
    int listenfd, connfd, i, maxi, sockfd;
    int nready;
    ssize_t n;
    socklen_t clilen;
    char buff[MAXLINE];
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in servaddr, cliaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(53101);

    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);
    client[0].fd = listenfd;    //set poll manager
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; ++i)
        client[i].fd = -1;      //initialize 
    maxi = 0;
    
    for ( ; ; ) {
        nready = poll(client, maxi + 1, INFTIM);

        if (client[0].revents & POLLRDNORM) {   //new connection
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

            for (i = 1; i < OPEN_MAX; ++i) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;  //save descriptor;
                    break;
                }
            }
            if (i == OPEN_MAX)
                exit(0);
            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;   //save max index
            if (--nready <= 0)
                continue;   //no more readable descriptor, can del
        }

        for (i = 1; i < maxi; ++i) {    //check all clients for data
            if ( (sockfd = client[i].fd) < 0 )
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( (n = read(sockfd, buff, MAXLINE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        Close(sockfd);  //reset by client
                        client[i].fd = -1;
                    } else
                        exit(0);
                } else if (n == 0) {
                    Close(sockfd);  //colsed by client
                    client[i].fd = -1;
                } else
                    Writen(sockfd, buff, n);
                
                if (--nready <= 0)
                    break;
            }
        }
    }
    return 0;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t  condition = PTHREAD_COND_INITIALIZER;  
int num = 0;

void * thread_work(void*)
{
    while(1)
    {      
        {
            printf("thread_work ");
            //pthread_mutex_lock(&mutex);
            guardMutex mt(&mutex);
            printf("the thread increase the number:%d, pthreadid:%lu\n", num, (unsigned long)pthread_self());
            num++;
            printf("the thread increase the number:%d, pthreadid:%lu\n", num, (unsigned long)pthread_self());
            //pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&condition);    //在unlock 之前
        }
        sleep(1);
    }
    return nullptr;
}

void * thread_work1(void*)
{
    int i = 10000;
    while(1)
    {
        char buff[128];
        struct timeval time;
        gettimeofday(&time, nullptr);
        LOG_DEBUG("name_%ld________%d_%lu", (time.tv_sec*1000 + time.tv_usec/1000), i, (unsigned long)pthread_self());
        sleep(1); 
        i += 2;
    }
    return nullptr;
}

void * thread_work2(void*)
{
    int i = 10000;
    while(1)
    {
        char buff[128];
        struct timeval time;
        gettimeofday(&time, nullptr);
        LOG_ERROR("bianbianabian____%ld________%d_%lu", (time.tv_sec*1000 + time.tv_usec/1000), i, (unsigned long)pthread_self());
        sleep(1); 
        i += 2;
    }
    return nullptr;
}


void writeFunc()
{
    log *t = log::getInstance();
    t->initialize("bian.log");

    printf("xxxxxxxxyyyyyyyyyyyyy\n");
    sleep(2);

    thread pro("logpro"); 
    pro.setRouter(thread_work1);
    pro.run();

    thread pro1("logpro"); 
    pro1.setRouter(thread_work1);
    pro1.run();

    thread pro2("logpro"); 
    pro2.setRouter(thread_work2);
    pro2.run();

    thread pro3("logpro"); 
    pro3.setRouter(thread_work2);
    pro3.run();

    thread the("logThread");
    the.setRouter(log::runTask, t);
    the.run();
    /*while (1) {
        char buff[128];
        struct timeval time;
        gettimeofday(&time, nullptr);
        snprintf(buff, sizeof(buff), "name_%ld________%d_%lu\n", (time.tv_sec*1000 + time.tv_usec/1000), 1, (unsigned long)pthread_self());
        LOG_DEBUG("name_%ld________%d_%lu", (time.tv_sec*1000 + time.tv_usec/1000), 1, (unsigned long)pthread_self());
        sleep(1); 
    }*/

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

void epollFunc()
{
    log *t = log::getInstance();
    t->initialize("bian.log");

    thread pro("logpro"); 
    pro.setRouter(thread_work1);
    pro.run();

    epoll *ep = new epoll();
    thread el("epoll");
    el.setRouter(epoll::runTask, ep);
    el.run();
}