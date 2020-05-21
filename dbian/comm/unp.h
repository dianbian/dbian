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

#define MAXLINE 4096
#define LISTENQ 1024
#define ZERO    0

#define SA struct sockaddr  //common use, switch with struct sockaddr_in

typedef void Sigfunc(int);

#define LOG_DEBUG(format, ...)     LogDebug(__FILE__, __LINE__, x, ##__VA_ARGS__);
#define LOG_ERROR(format, ...)       LogError(__FILE__, __LINE__, x, ##__VA_ARGS__); 
void LogDebug(const char* pFile, int iLine, LPCTSTR pFormat, ...);
void LogError(const char* pFile, int iLine, LPCTSTR pFormat, ...);

