#pragma once
#include "comm/unp.h"

int
Socket(int family, int type, int protocol)
{
    int bindFd = socket(family, type, protocol);
    if (bindFd < 0) {
        printf("socket error\n");
        return -1;
    }
    return bindFd;
}

int 
Bind(int bindFd, const SA *servAddr, socklen_t addrLen)
{
    int ret = bind(bindFd, servAddr, addrLen);
    if (ret < 0) {
        return -1;
    }
    return ret;
}

int
Listen(int sockFd, int backLog)
{
    char *ptr;
    if ((ptr = getenv("LISTEN")) != NULL)
        backLog = atoi(ptr);
    printf("backlog=%d\n", backLog);
    int ret = listen(sockFd, backLog);
    if (ret < 0) {
        return -1;
    }
    return 0;
}

int 
Connect(int sockFd, SA *servAddr, socklen_t addrLen)
{
    int ret = connect(sockFd, servAddr, addrLen);
    if (ret < 0) {
        printf("conn error\n");
        return -1;
    }
    return ret;
}

int 
Accept(int sockFd, SA *cliAddr, socklen_t *addrLen)
{
    int connFd = accept(sockFd, cliAddr, addrLen); //accept4  blocking
    if (connFd < 0) {
        return -1;
    }
    return connFd;
}

int
Close(int connFd)
{
    int ret = close(connFd);
    if (ret < 0){
        return -1;
    }
    return 0;
}

int
Writen(int connFd, const void *buff, size_t n)
{
    size_t ret = write(connFd, buff, n);
    if (ret != n) {
        return -1;
    }
    return ret;
}

int 
Inet_pton(int family, const char *ipPtr, void *addrPtr)
{   //handle ipv4/ipv6, like inet_aton
    int ret = inet_pton(family, ipPtr, addrPtr);
    if (ret <= 0) {
        return -1;
    }
    return ret; //1 success, -1 error, 0 input is invaild
}

int
Read(int connFd, void *buff, size_t n)
{
    size_t ret = read(connFd, buff, n);
    if (ret != n) {
        return -1;
    }
    return ret;
}

static int initTcpSocket(const char* ip, int port) {
    int listenfd;
    int backLog = LISTENQ;
    char *ptr;
    int ret;
    int opt = 1;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        return -1;
    }
    //设置端口复用
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0)
        return -1;
    //禁用Nagle算法
    if (setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0 )
        return -1;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    ret = bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        return -1;
    }

    if ((ptr = getenv("LISTEN")) != NULL)
        backLog = atoi(ptr);
    ret = listen(listenfd, backLog);
    if (ret < 0) {
        return -1;
    }
    LOG_DEBUG("liste on %s, %d", ip, port);
    return listenfd;
}
