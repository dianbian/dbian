#include "comm/unp.h"
#include "socket.h"

//ssize_t in 32bit like int, in 64bit lit long int
ssize_t     //read n bytes from a descriptor
readn(int sockFd, void *vptr, size_t n) 
{
    size_t nLeft = n;
    int nRead = 0;
    char *ptr = (char *)vptr;
    while (nLeft > 0) {
        if ( (nRead = read(sockFd, ptr, nLeft)) < 0) {
            if (errno == EINTR)
                nRead = 0;
            else
                return -1;  //error
        } else if (nRead == 0)
            break;
        nLeft -= nRead;
        ptr += nRead;   //ptr go forward
    }
    return n - nLeft;
}

ssize_t     //write n bytes to a descriptor
writen(int sockFd, const void *vptr, size_t n)
{
    size_t nLeft = n;
    int nWriten;
    const char* ptr = (const char*)vptr;
    while (nLeft > 0) {
        if ( (nWriten = write(sockFd, ptr, nLeft)) <= 0) {
            if (nWriten < 0 && errno == EINTR)
                nWriten = 0;
            else
                return -1;  //error
        }
        nLeft -= nWriten;
        ptr += nWriten;
    }
    return n;
}

ssize_t
readline(int sockFd, void *vptr, size_t maxLen)
{
    size_t n, rc;
    char c, *ptr = (char *)vptr;
    for (n = 1; n < maxLen; n++) {
    again:
        if ( (rc = read(sockFd, &c, 1)) == 1 ) {
            *ptr++ = c;
            if (c == '\n')
                break;  //Enter return
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;   //EOF
        } else {
            if (errno == EINTR)
                goto again;
            return -1;  //error
        }
    }
    *ptr = 0;   //last bit set 0
    return n;
}

void
str_cli(FILE *fp, int sockFd)
{
    char sendLine[MAXLINE], recvLine[MAXLINE];

    snprintf(sendLine, sizeof(sendLine), "%s", "alksdja;lksdakncmxzcn;lkasjrwpoierju[pwdlkasmd;laKSJD;LAKSDJF;LASKDJF;LASDKJF;ALSDKFJN;ALSDKJFL;K");
    printf("%s\n", sendLine);
    Writen(sockFd, sendLine, strlen(sendLine));
    return;
    while (fgets(sendLine, MAXLINE, fp) != NULL) {
        
        if (readline(sockFd, recvLine, MAXLINE) == 0)
            return;

        fputs(recvLine, stdout);
    }
}

void
str_echo(int sockFd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while ( (n = read(sockFd, buf, MAXLINE)) > 0 )
        Writen(sockFd, buf, n);
    
    printf("recv = %s\n", buf);
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        return; //error
}

void    //udp serv
dg_echo(int sockFd, SA *pcliAddr, socklen_t cliLen)
{
    int n;
    socklen_t len;
    char mesg[MAXLINE];
    char buff[MAXLINE];

    for( ; ; ) {
        len = cliLen;
        n = recvfrom(sockFd, mesg, MAXLINE, 0, pcliAddr, &len);
        struct sockaddr_in cliAddr = *(struct sockaddr_in *)pcliAddr;
        printf("connection from %s, port %d\n", 
            inet_ntop(AF_INET, &cliAddr.sin_addr, buff, sizeof(buff)),
            ntohs(cliAddr.sin_port));
        printf("recv = %s\n", mesg);
        sendto(sockFd, mesg, n, 0, pcliAddr, len);
    }
}

void    //udp cli
dg_cli(FILE *fp, int sockFd, SA *pservAddr, socklen_t servLen)
{
    int n;
    char sendLine[MAXLINE], recvLine[MAXLINE + 1];

    while (fgets(sendLine, MAXLINE, fp) != NULL) {
        sendto(sockFd, sendLine, strlen(sendLine), 0, pservAddr, servLen);
        
        n = recvfrom(sockFd, recvLine, MAXLINE, 0, NULL, NULL);

        recvLine[n] = 0;    //last bit set 0
        fputs(recvLine, stdout);
    }
}