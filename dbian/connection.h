#pragma once
/*
    create by bianbian, 2020/5/25
*/

#include "comm/linkedList.h"
#include "comm/unp.h"
#include "msg.h"

class connection
{
private:
    typedef struct data {
        size_t type;
        size_t len;     //日志里面特定长度，在con里也是长度
        size_t index;   //起始index 一个index + 1 == sum 终止
        size_t idSum;   //总数
        char data[DATALENGTH];  //if input size > DATALENGTH, malloc
    }dataNode;

    typedef struct node {
        struct node *pPrev;
        struct node *pNext;
        dataNode data;
    }List, *pNode;

    /* data */
    int m_fd;
    int m_flag; //是否可用
    int m_headLen;  //消息头长
    int m_msgLen;   //内容长
    linkedList<List> m_recvBuf;   //收队列
    linkedList<List> m_sendBuf;   //发队列
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

    int getFd() const {
        return m_fd;
    }

    void setFd(int fd) {
        m_fd = fd;
    }

    void setAddr(struct sockaddr_in& cliAddr) {
        m_cliAddr = cliAddr;
    }

    int readMsg() {
        m_headLen = 0;
        m_msgLen = 0;
        int msgLen = 0;
        msgHeader msg;
        m_headLen = readn(m_fd, &msg, HEADLEN);
        if (m_headLen != HEADLEN)   //固定长度
            return -1;
        m_msgLen = msg.msgLen - HEADLEN;
        /*msgLen = readn(m_fd, buf, m_msgLen);    //变长
        if (m_msgLen != msgLen)
            goto bprint;*/
        size_t blockSize = m_msgLen / DATALENGTH + 1; //至少一个缓存块使用
        LOG_DEBUG("fd = %d, msgLen = %ld, blockSize = %d", m_fd, m_msgLen, blockSize);
        for (size_t i = 0; i < blockSize; ++i) {
            pNode p = m_recvBuf.getNodeNull();
            p->data.type = msg.msgType;
            p->data.index = i + 1;
            p->data.idSum = blockSize;
            memset(p->data.data, 0, DATALENGTH);
            if (i + 1 == blockSize) {   //特殊处理, 长度也特殊
                msgLen = readn(m_fd, p->data.data, m_msgLen - (i * DATALENGTH));
                p->data.len = m_msgLen - (i * DATALENGTH);
                printf("xxxxxxxxx, %d, %lu\n", msgLen, i);
            }
            else {
                p->data.len = DATALENGTH;
                msgLen = readn(m_fd, p->data.data, DATALENGTH); //i < blockSize
                printf("yyyyyyyyy, %d, %lu\n", msgLen, i);
            }
            //TODO出错处理
        }
        return msgLen;
    }

    int getMsg(char *buff, size_t& type, size_t maxSize) {
        size_t len = 0; //必不小于0
        size_t i = 0;
        m_recvBuf.setTail(m_recvBuf.getHead());   //中位指针回溯
        do {
            pNode p = m_recvBuf.getNode();
            if (p->data.index + 1 == p->data.idSum) {   //特殊
                memcpy(buff + len, p->data.data, p->data.len);   //拷贝指针偏移
                len += p->data.len;
                printf("mmmmmmmmmmmmmmm, %lu, %lu\n", len, i);
                break;
            }
            printf("ooooooooooooooooo, %lu, %lu\n", len, i);
            memcpy(buff + len, p->data.data, p->data.len);
            len += p->data.len;
            if (maxSize < len)
                break;
            i++;
        }while(1);  //一直读 index + 1 == idSum break
        return len;
    }
};
