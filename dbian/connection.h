#pragma once
/*
    create by bianbian, 2020/5/25
*/

#include "comm/linkedList.h"
#include "comm/unp.h"
#include "msg.h"
#include "communicate.h"

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
    char m_buff[MAXLINE];
    char m_msgSend[MSGLEN];  //4M?

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

    const char* getCliIp() {
        return inet_ntop(AF_INET, &m_cliAddr.sin_addr, m_buff, sizeof(m_buff));
    }

    int getCliPort() {
        return ntohs(m_cliAddr.sin_port);
    }

    int readMsg() {
        m_headLen = 0;
        m_msgLen = 0;
        int msgLen = 0;
        msgHeader msg;
        m_headLen = readn(m_fd, &msg, HEADLEN);
        if (m_headLen != HEADLEN)   //固定长度
            return LOSSONE;
        m_msgLen = msg.msgLen - HEADLEN;
        size_t blockSize = m_msgLen / DATALENGTH + 1; //至少一个缓存块使用
        LOG_DEBUG("fd = %d, msgLen = %ld, blockSize = %d", m_fd, m_msgLen, blockSize);
        m_recvBuf.setTail(m_recvBuf.getHead());   //中位指针回溯
        for (size_t i = 0; i < blockSize; ++i) {
            pNode p = m_recvBuf.getNodeNull();  //非空
            p->data.type = msg.msgType;
            p->data.index = i;  //从0开始 max(i) + 1 == blockSize
            p->data.idSum = blockSize;
            memset(p->data.data, 0, DATALENGTH);
            if (i + 1 == blockSize) {   //特殊处理, 长度也特殊
                msgLen = readn(m_fd, p->data.data, m_msgLen - (i * DATALENGTH));
                p->data.len = m_msgLen - (i * DATALENGTH);
                printf("xxxxxxxxx, %d, %lu, index= %lu, idSum = %lu\n", msgLen, i, p->data.index, p->data.idSum);
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

    int getRecvMsg(char *buff, size_t& type, size_t maxSize) {
        size_t len = 0; //必不小于0
        size_t i = 0;
        m_recvBuf.setTail(m_recvBuf.getHead());   //中位指针回溯
        do {
            pNode p = m_recvBuf.getNode();  //理论上不会null
            if (p == nullptr) {
                printf("continue   \n");
                continue;
            }
            if (p->data.index + 1 == p->data.idSum) {   //特殊
                memcpy(buff + len, p->data.data, p->data.len);   //拷贝指针偏移
                len += p->data.len;
                p->data.len = 0;
                type = p->data.type;
                printf("mmmmmmmmmmmmmmm, %lu, %lu\n", len, i);
                break;
            }
            printf("rrrrrrr, %lu, len =%lu, index= %lu, idSum = %lu\n", i, len, p->data.index, p->data.idSum);
            memcpy(buff + len, p->data.data, p->data.len);
            len += p->data.len;
            p->data.len = 0;
            if (maxSize < len)  //TODO  danger
                break;
            i++;
        }while(1);  //一直读 index + 1 == idSum break
        return len;
    }

    int sendMsg(const char* buff, size_t type, size_t len) {
        int retLen = 0;
        if (len != strlen(buff))
            return LOSSONE;
        if (len <= 0)
            return LOSSONE;
        size_t blockSize = len / DATALENGTH + 1; //至少一个缓存块使用
        LOG_DEBUG("fd = %d, len = %ld, blockSize = %d", m_fd, len, blockSize);
        m_sendBuf.setTail(m_sendBuf.getHead());   //中位指针回溯
        for (size_t i = 0; i < blockSize; ++i) {
            pNode p = m_sendBuf.getNodeNull();  //非空
            p->data.type = type;
            p->data.index = i;  //从0开始 max(i) + 1 == blockSize
            p->data.idSum = blockSize;
            memset(p->data.data, 0, DATALENGTH);
            if (i + 1 == blockSize) {   //特殊处理, 长度也特殊
                memcpy(p->data.data, buff, len - (i * DATALENGTH));
                p->data.len = len - (i * DATALENGTH);
                printf("uuuuuuuuuuuuuuu, %lu, %lu, index= %lu, idSum = %lu\n", len, i, p->data.index, p->data.idSum);
            }
            else {
                p->data.len = DATALENGTH;
                memcpy(p->data.data, buff, DATALENGTH); //i < blockSize
                printf("pppppppppppppppp, %lu, %lu\n", len, i);
            }
            //TODO出错处理
        }
        return 0;
    }

    int writeMsg() {
        size_t len = HEADLEN; //必不小于0
        size_t i = 0;
        m_sendBuf.setTail(m_sendBuf.getHead());   //中位指针回溯
        memset(m_msgSend, 0, MSGLEN);

        msgHeader msgh;
        memcpy(m_msgSend, &msgh, HEADLEN);
        do {
            pNode p = m_sendBuf.getNode();  //理论上不会null
            if (p == nullptr) {
                printf("continue   \n");
                continue;
            }
            if (p->data.index + 1 == p->data.idSum) {   //特殊
                memcpy(m_msgSend + len, p->data.data, p->data.len);   //拷贝指针偏移
                len += p->data.len;
                p->data.len = 0;
                msgh.msgType = p->data.type;
                msgh.msgLen = len;
                printf("jjjjjjjjjjjjjjj, %lu, %lu\n", len, i);
                break;
            }
            printf("kkkkkkkkkkkkkkk, %lu, len =%lu, index= %lu, idSum = %lu\n", i, len, p->data.index, p->data.idSum);
            memcpy(m_msgSend + len, p->data.data, p->data.len);
            len += p->data.len;
            p->data.len = 0;
            if (MSGLEN < len)  //TODO  danger
                return LOSSONE;
            i++;
        }while(1);  //一直读 index + 1 == idSum break
        
        if (len > HEADLEN) {
            printf("%0x, len = %lu, bufflen = %lu, msglen = %d\n", msgh.msgType, HEADLEN, len, msgh.msgLen);
            len = writen(m_fd, m_msgSend, msgh.msgLen);
        }
        return len;
    }
};
