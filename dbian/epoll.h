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
    std::vector<connection *> m_connVec;    //重复利用
public:
    epoll(size_t nSize = LISTENQ) {
        m_epollFd = 0;
        m_listenFd = 0;
        for (size_t i = 0; i < nSize; ++i) {
            connection *cc = new connection();
            m_connVec.push_back(cc);
        }
    }

    ~epoll() {    
    }
    
    bool initialize() {
        //创建epoll int epoll_create(int size);
        m_listenFd = initTcpSocket("127.0.0.1", 53101);
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
        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
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
        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        event.data.ptr = conn;
        setnonblocking(fd);
    
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }

    bool delFd(int fd) {
        int ret = epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }

    
    bool waitFd(std::vector<uintptr_t> &list, int ms_timeout = 3000) {
        //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        struct epoll_event evs[LISTENQ];
        int nfds = epoll_wait(m_epollFd, evs, LISTENQ, ms_timeout);
        if (nfds <= 0) {
            LOG_ERROR("epoll wait error");
            return false;
        }
        for (int i = 0; i < nfds; ++i) {
            if (evs[i].data.fd == m_listenFd)
                accept();
            else
                list.push_back((uintptr_t)evs[i].data.ptr);
        }
        return true;
    }

    void accept() {
        int newFd;
        LOG_ERROR("epoll accept");
        memset(&m_addr, 0, sizeof(m_addr));
        while ( (newFd = ::accept(m_listenFd, (SA *)&m_addr, &m_len)) != 0) {
            for (auto& conn : m_connVec) {
                if (conn->getFlag()) {
                    conn->setFd(newFd);
                    conn->setFlag(1);
                    conn->setAddr(m_addr);
                    LOG_ERROR("epoll wait error");
                    addConnetion(newFd, conn);
                    return;
                }
            }
            connection *conn = new connection(newFd, 1, m_addr);
            addConnetion(newFd, conn);
        }
        return;
    }

    void dealHandle() {
        while (1) {
            std::vector<uintptr_t> list;
            if (!waitFd(list)) {
                sleep(1);
                continue;
            }
            LOG_ERROR("wait");
            for (auto &connPtr : list) {
                connection *conn = (connection *)connPtr;
                if (conn != nullptr) continue;
                else {
                    char buf[2048];
                    readn(conn->getFd(), buf, 2048);
                    printf("recv = %s\n", buf);
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