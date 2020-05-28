#pragma once 
#include "../comm/unp.h"
#include "../communicate.h"

int
cli_main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    //禁用Nagle算法
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0 )
        return -1;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TESTPORT);
    Inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    
    str_cli(stdin, sockfd);
    
    sleep(1);
    Close(sockfd);
    return 0;
}

int
cli_main_udp(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(TESTPORT);
    Inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    
    dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
    
    Close(sockfd);
    return 0;
}