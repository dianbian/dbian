#pragma once

#include <sys/epoll.h>
#include <vector>

#include "comm/unp.h"
#include "connection.h"
#include "socket.h"

class epoll {
private:
    int m_epollRecvFd;
    int m_epollSendFd;
    int m_listenFd;
    size_t m_type;
    struct sockaddr_in m_addr;
    socklen_t m_len; 
    epoll_event *m_evtRecv;
    epoll_event *m_evtSend;
    char m_buff[MAXLINE];
    char m_msgRecv[MSGLEN];  //4M?
    
    std::vector<uintptr_t> m_connEv;    //实际有效事件
    std::vector<connection *> m_connVec;    //重复利用 初始1024
    //TODO
    //std::map<int, connection *> m_connMap;  //加速查找
    size_t m_connSum;   //实际有效连接
    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;  
    pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;

public:
    epoll(size_t nSize = LISTENQ) {
        m_epollRecvFd = 0;
        m_epollSendFd = 0;
        m_listenFd = 0;
        m_connSum = 0;
        pthread_mutex_init(&m_mutex, NULL);
	    pthread_cond_init(&m_cond, NULL);
        for (size_t i = 0; i < nSize; ++i) {
            connection *cc = new connection();
            m_connVec.push_back(cc);
        }
    }

    ~epoll() {    
        pthread_mutex_destroy(&m_mutex);
	    pthread_cond_destroy(&m_cond);
    }
    
    bool initialize() {
        //创建epoll int epoll_create(int size);
        m_listenFd = initTcpSocket("127.0.0.1", TESTPORT);
        if (m_listenFd < 0)
            return false;
        m_epollRecvFd = epoll_create(1);
        if (m_epollRecvFd < 0)
            return false;
        m_epollSendFd = epoll_create(1);
        if (m_epollSendFd < 0)
            return false;
        struct sockaddr_in m_servAddr;
        if (!addConnetion(m_epollRecvFd, m_listenFd, nullptr, EPOLLIN)) {
            return false;
        }
        m_connSum++;
        return true;
    }

    void setnonblocking(int fd) {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
        return;
    }

    bool resetConnection(int epollfd, int fd, connection *conn, int flag) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLET | EPOLLONESHOT | flag;
        event.data.ptr = conn;
        int ret = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
        if (ret < 0) {
            return false;
        }
        return true;
    }

    bool addFd(int epollfd, int fd) {
        //不能将监听端口listenfd设置为EPOLLONESHOT否则会丢失客户连接
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLET | EPOLLIN;
        setnonblocking(fd);
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0) {
            return false;
        }
        return true;
    }

    bool addConnetion(int epollfd, int fd, connection *conn, int flag) {
        struct epoll_event event;
        event.data.fd = fd;
        //event.events = EPOLLET | EPOLLONESHOT | flag;
        event.events = EPOLLET | EPOLLIN;
        event.data.ptr = 0;
        setnonblocking(fd);
        LOG_DEBUG("xxxxx ,fd = %d, ev=%0x", fd, event.events);
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0) {
            LOG_ERROR("epoll ctl fd error");
            return false;
        }
        return true;
    }

    bool delFd(int epollfd, int fd) {
        int ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            LOG_ERROR("epoll ctl fd error"); 
            return false;
        }
        return true;
    }

    
    size_t waitRecvFd(std::vector<uintptr_t> &list, int ms_timeout = -1) {
        //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        memset(m_evtRecv, 0, LISTENQ * sizeof(epoll_event));
        int nfds = epoll_wait(m_epollRecvFd, m_evtRecv, LISTENQ, ms_timeout);
        if (nfds <= 0) {
            return ZERO;
        }
        LOG_DEBUG("nfds = %d epoll event", nfds);
        //TODO 拆分线程accpet
        for (int i = 0; i < nfds; ++i) {
            LOG_DEBUG("EPOLLIN ,fd = %d, ev=%0x", m_evtRecv[i].data.fd, m_evtRecv[i].events);
            if ((m_evtRecv[i].events & EPOLLERR) || (m_evtRecv[i].events & EPOLLHUP) ||  
              (!(m_evtRecv[i].events & EPOLLIN))) {
                LOG_ERROR("EPOLLERR");
                continue;
            }
            else if (m_evtRecv[i].data.fd == m_listenFd) {
                accept();
                continue;
            }
            else if ((m_evtRecv[i].events & EPOLLIN) || (m_evtRecv[i].events & EPOLLPRI)) {
                list.push_back((uintptr_t)m_evtRecv[i].data.ptr);
            }
            else{
                LOG_ERROR("EPOLLIN ,fd = %d", m_evtRecv[i].data.fd);
            }
        }
        return list.size();
    }

    size_t waitSendFd(std::vector<uintptr_t> &list, int ms_timeout = -1) {
        int nfds = epoll_wait(m_epollSendFd, m_evtSend, LISTENQ, ms_timeout);
        if (nfds <= 0) {
            return false;
        }
        LOG_DEBUG("nfds = %d epoll event", nfds);
        //TODO 拆分线程accpet
        for (int i = 0; i < nfds; ++i) {
            if ((m_evtSend[i].events & EPOLLOUT)) {
                LOG_DEBUG("EPOLLOIUT");
                list.push_back((uintptr_t)m_evtSend[i].data.ptr);
            }
        }
        return list.size();
    }

    void accept() {
        int newFd;
        memset(&m_addr, 0, sizeof(m_addr));
        memset(m_buff, 0, sizeof(m_buff));
        while ( (newFd = ::accept(m_listenFd, (SA *)&m_addr, &m_len)) > 0) {
            bool flag = true;
            for (auto& conn : m_connVec) {
                if (conn->getFlag()) {
                    conn->setFd(newFd);
                    conn->setFlag(1);
                    conn->setAddr(m_addr);
                    addConnetion(m_epollRecvFd, newFd, conn, EPOLLIN);
                    m_connSum++;
                    flag = false;
                    LOG_DEBUG("epoll accept connection from %s, port %d, fd %d, sum = %ld", 
                        conn->getCliIp(), conn->getCliPort(), newFd, m_connSum);
                    break;
                }
            }
            if (flag) {
                connection *conn = new connection(newFd, 1, m_addr);
                addConnetion(m_epollRecvFd, newFd, conn, EPOLLIN);
                m_connVec.push_back(conn);
                m_connSum++;
            }
        }
        return;
    }

    connection *getConn(int fd) {
        for (auto &conn : m_connVec) {
            if (!conn->getFlag() && conn->getFd() == fd)
                return conn;
        }
    }

    void loop() {   //主循环处理接受
        m_evtRecv = new epoll_event[LISTENQ];
        std::vector<uintptr_t> list;
        while (1) {
            if (waitRecvFd(list) == ZERO) {
                sleep(1);
                continue;
            }
            guardMutex gm(&m_mutex);
            if (list.size() > 0 && m_connEv.size() == ZERO) {
                m_connEv.swap(list);    //消耗应该很小吧?
                pthread_cond_signal(&m_cond);
            }
        }
        return;
    }

    void readMsg() {
        while (1) {
            guardMutex gm(&m_mutex);
            if (m_connEv.size() == ZERO) {
                pthread_cond_wait(&m_cond, &m_mutex);
            }
            for (auto &connPtr : m_connEv) {
                connection *conn = (connection *)connPtr;
                if (conn == nullptr) continue;
                else {
                    memset(m_msgRecv, 0, MSGLEN);                    
                    int len1 = conn->readMsg();
                    int len2 = conn->getRecvMsg(m_msgRecv, m_type, MSGLEN); //TODO thread read
                    handleNetMsg(conn, m_msgRecv, m_type, len2);
                    resetConnection(m_epollRecvFd, conn->getFd(), conn, EPOLLIN);
                }
            }
            std::vector<uintptr_t> tmpVec;
            m_connEv.swap(tmpVec);
        }
    }

    //虚函数回调or注册 TODO
    virtual int handleNetMsg(connection *conn, const char *buff, size_t type, size_t len) {
        if (conn == nullptr)
            return -1;
        if (type == REQUESE1) {
            LOG_DEBUG("fd = %d, type = %0x, len= %d, buf = %s", conn->getFd(), type, len, buff);
            sendMsg(conn, buff, REQUESE2, len);
        }
        else if (type == REQUESE3) {
            LOG_DEBUG("fd = %d, type = %0x, len= %d, buf = %s", conn->getFd(), type, len, buff);
            sendMsg(conn, buff, REQUESE3, len);
        }
        else {
            LOG_ERROR("fd = %d, type = %0x, len= %d, buf = %s", conn->getFd(), type, len, buff);
            //close();
        }
        return 0;
    }

    //close();

    int sendMsg(connection *conn, const char* buff, size_t type, size_t len) {
        LOG_DEBUG("fd = %d, type = %0x, len= %d, buf = %s", conn->getFd(), type, len, buff);
        if (conn == nullptr)
            return -1;
        int retLen = conn->sendMsg(buff, type, len);
        bool isRegist = false;
        if (retLen == (int)len) {
            //add Fd
            LOG_DEBUG("ttttttttttttttt");
            if (conn->getSendFlag() == false) 
                isRegist = addConnetion(m_epollSendFd, conn->getFd(), conn, EPOLLOUT);
            else
                isRegist = resetConnection(m_epollSendFd, conn->getFd(), conn, EPOLLOUT);
            if (isRegist)
                conn->setSendFlag(true);
            else
                ;//close()
        }
        return retLen;
    }

    void dealSendMsg() {   //线程处理发送
        m_evtSend = new epoll_event[LISTENQ];
        while (1) {
            std::vector<uintptr_t> list;
            if (waitSendFd(list) == ZERO) {
                continue;
            }
            LOG_DEBUG("size = %lu", list.size());
            for (auto &connPtr : list) {
                connection *conn = (connection *)connPtr;
                if (conn == nullptr) continue;
                else {
                    int len1 = conn->writeMsg();
                    LOG_DEBUG("len = %lu", len1);
                    //delFd(m_epollSendFd, conn->getFd());
                }
            }
        }
        return;
    }

    static void *dealEpoll(void *arg) {
        epoll *tmp = (epoll*)arg;
        tmp->loop();
        return nullptr;
    }

    static void *dealMsg(void *arg) {
        epoll *tmp = (epoll*)arg;
        tmp->readMsg();
        return nullptr;
    }

    static void *dealSend(void *arg) {
        epoll *tmp = (epoll*)arg;
        tmp->dealSendMsg();
        return nullptr;
    }
};