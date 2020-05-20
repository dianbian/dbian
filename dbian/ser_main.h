#pragma once
#include "comm/unp.h"
#include "socket.h"
#include "signal.h"
#include "communicate.h"

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