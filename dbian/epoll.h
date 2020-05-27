#pragma once

#include <sys/epoll.h>
#include <vector>

#include "comm/unp.h"
#include "connection.h"


class epoll {
private:
    int m_epollFd;
    int m_listenFd;
    struct sockaddr_in m_addr;
    socklen_t m_len; 
    char m_buff[MAXLINE];
    epoll_event *m_evt;
    std::vector<connection *> m_connVec;    //重复利用 初始1024
    size_t m_connSum;   //实际有效连接
public:
    epoll(size_t nSize = LISTENQ) {
        m_epollFd = 0;
        m_listenFd = 0;
        m_connSum = 0;
        for (size_t i = 0; i < nSize; ++i) {
            connection *cc = new connection();
            m_connVec.push_back(cc);
        }
    }

    ~epoll() {    
    }
    
    bool initialize() {
        //创建epoll int epoll_create(int size);
        m_listenFd = initTcpSocket("127.0.0.1", TESTPORT);
        if (m_listenFd < 0)
            return false;
        m_epollFd = epoll_create(1);
        if (m_epollFd < 0)
            return false;
        struct sockaddr_in m_servAddr;
        connection *conn = new connection(m_listenFd, 1, m_servAddr);
        if (!addFd(m_listenFd, conn)) {
            delete conn;
            return false;
        }
        m_connVec.push_back(conn);
        m_connSum++;
        return true;
    }

    void setnonblocking(int fd) {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
        return;
    }

    void reset_oneshot(int fd, connection *conn) {
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
        event.data.ptr = this;
        epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &event);
    }

    bool addFd(int fd, connection *conn) {
        //定义事件
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
        setnonblocking(fd);
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0) {
            return false;
        }
        return true;
    }

    bool addConnetion(int fd, connection *conn) {
        //定义事件
        struct epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
        event.data.ptr = conn;
        setnonblocking(fd);
    
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0) {
            printf("epoll ctl fd error 111111111 \n");
            exit(0);
            return false;
        }
        return true;
    }

    bool delFd(int fd) {
        int ret = epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            printf("epoll ctl fd error\n");
            exit(0);
            return false;
        }
        return true;
    }

    
    size_t waitFd(std::vector<uintptr_t> &list, int ms_timeout = 3000) {
        //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        int nfds = epoll_wait(m_epollFd, m_evt, LISTENQ, ms_timeout);
        if (nfds <= 0) {
            return false;
        }
        for (int i = 0; i < nfds; ++i) {
            if ((m_evt[i].events & EPOLLERR) || (m_evt[i].events & EPOLLHUP) ||  
              (!(m_evt[i].events & EPOLLIN))) {
                printf("EPOLLERR\n");
                continue;
            }
            else if (m_evt[i].data.fd == m_listenFd)
                accept();
            else if ((m_evt[i].events & EPOLLIN) || (m_evt[i].events & EPOLLPRI)) {
                printf("EPOLLIN\n");
                list.push_back((uintptr_t)m_evt[i].data.ptr);
            }
        }
        LOG_DEBUG("nfds = %d vec size= %ld", nfds, list.size());
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
                    addConnetion(newFd, conn);
                    m_connSum++;
                    flag = false;
                    printf("epoll accept connection from %s, port %d, fd %d, sum = %ld\n", 
                        inet_ntop(AF_INET, &m_addr.sin_addr, m_buff, sizeof(m_buff)), 
                        ntohs(m_addr.sin_port), newFd, m_connSum);
                    break;
                }
            }
            if (flag) {
                connection *conn = new connection(newFd, 1, m_addr);
                addConnetion(newFd, conn);
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
    void dealHandle() {
        char buf[2048];
        m_evt = new epoll_event[LISTENQ];
        while (1) {
            std::vector<uintptr_t> list;
            if (waitFd(list) == 0) {
                sleep(1);
                continue;
            }
            for (auto &connPtr : list) {
                connection *conn = (connection *)connPtr;
                if (conn == nullptr) continue;
                else {
                    memset(buf, 0, 2048);
                    int len = Read(conn->getFd(), buf, 2048);
                    if (len <= 0) {
                        conn->setFlag(ZERO);
                        close(conn->getFd());
                        m_connSum--;
                        printf("fd = %d, close, sum=%ld\n", conn->getFd(), m_connSum);    
                    }
                    else
                        printf("fd = %d, sum=%ld, recv = %s\n", conn->getFd(), m_connSum, buf);
                }
            }
        }
    }

    static void *runTask(void *arg) {
        epoll *tmp = (epoll*)arg;
        tmp->dealHandle();
        return nullptr;
    }
};