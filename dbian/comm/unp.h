#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <limits.h>
#include <poll.h>
#include <sys/sysctl.h>
#include <stdarg.h>
#include "log.h"

#define MAXLINE 4096
#define LISTENQ 1024
#define MAXBUFF 2048

#define DATE_FORMAT "[%04d-%02d-%02d %02d:%02d:%02d]"

#define SA struct sockaddr  //common use, switch with struct sockaddr_in

typedef void Sigfunc(int);

#define LOG_DEBUG(format, args...)     LogDebug(__LINE__, __FUNCTION__, format, ##args)
#define LOG_ERROR(format, args...)     LogError(__LINE__, __FUNCTION__, format, ##args)

int vspf(char *buffer, char *format, ...) {
    int cnt;
    va_list argptr; //声明一个转换参数的变量
    va_start(argptr, format); //初始化变量  
    cnt = vsnprintf(buffer, MAXBUFF, format, argptr);
    va_end(argptr); //结束变量列表,和va_start成对使用  
    return cnt;
}

void LogDebug(int iLine, const char *pFunc, const char *format, ...) {
    time_t nowtime = time(NULL);
    tm *now = localtime(&nowtime);
    char buffData[64];
    char buffCont[MAXBUFF];
    char buffer[MAXBUFF];
    
    va_list argptr; //声明一个转换参数的变量
    va_start(argptr, format); //初始化变量  
    vsnprintf(buffCont, MAXBUFF, format, argptr);
    va_end(argptr); //结束变量列表,和va_start成对使用  

    snprintf(buffData, sizeof(buffData), DATE_FORMAT, now->tm_year+1900, now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    snprintf(buffer, sizeof(buffer), "%s #DEBUG# %02d %s %s \n", buffData, iLine, pFunc, buffCont);
    log::getInstance()->produce(buffer);
}

void LogError(int iLine, const char *pFunc, const char *format, ...) {
    time_t nowtime = time(NULL);
    tm *now = localtime(&nowtime);
    char buffData[64];
    char buffCont[MAXBUFF];
    char buffer[MAXBUFF];
    
    va_list argptr; //声明一个转换参数的变量
    va_start(argptr, format); //初始化变量  
    vsnprintf(buffCont, MAXBUFF, format, argptr);
    va_end(argptr); //结束变量列表,和va_start成对使用  

    snprintf(buffData, sizeof(buffData), DATE_FORMAT, now->tm_year+1900, now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    snprintf(buffer, sizeof(buffer), "%s #ERROR# %02d %s %s \n", buffData, iLine, pFunc, buffCont);
    log::getInstance()->produce(buffer);
}


/*
printf("%s\n",__FILE__);
printf("%s\n",__func__);
printf("%d\n",__LINE__);
printf("%s %s\n",__DATE__, __TIME__);


    time_t nowtime = time(NULL);
 tm *now = localtime(&nowtime);
 printf("now: %04d-%02d-%02d %02d:%02d:%02d\n\n",
  now->tm_year+1900, now->tm_mon+1, now->tm_mday,
  now->tm_hour, now->tm_min, now->tm_sec, now->tm_usec);
*/
