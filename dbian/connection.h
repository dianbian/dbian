#pragma once
/*
    create by bianbian, 2020/5/25
*/

#include "comm/linkedList.h"

class connection
{
private:
    /* data */
    int m_fd;
    int m_flag; //是否可用
    linkedList m_recvBuf;   //收队列
    linkedList m_sendBuf;   //发队列
    struct sockaddr_in m_cliAddr;

public:
    connection() {
        m_fd = ZERO;
        m_flag = ZERO;
        m_recvBuf.initList();
        m_sendBuf.initList();
    }

    connection(int fd, int flag, struct sockaddr_in cliAddr) : 
            m_fd(fd), m_flag(flag), m_cliAddr(cliAddr) {
        m_recvBuf.initList();
        m_sendBuf.initList();
    }
    
    ~connection() {
    }

    bool getFlag() {
        return m_flag == ZERO;  //等于0为true, 可用
    }
    
    void setFlag(int flag) {
        m_flag = flag;
    }

    int getFd() {
        return m_fd;
    }

    void setFd(int fd) {
        m_fd = fd;
    }

    void setAddr(struct sockaddr_in& cliAddr) {
        m_cliAddr = cliAddr;
    }
};
