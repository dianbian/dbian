#pragma once

#include <sys/epoll.h>

class Epoll {
private:
    int m_epfd;
public:
    bool initialize() {
        //创建epoll int epoll_create(int size);
        m_epfd = epoll_create(1);
        if (m_epfd < 0) {
            perror("epoll create error");
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

    void reset_oneshot(int epollfd, int fd) {
        epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
    }

    bool addFd(int fd, uint32_t events = 0) {
        //定义事件
        struct epoll_event ev;
        ev.events = EPOLLIN | events | EPOLLONESHOT;
        ev.data.fd = fd;

        setnonblocking(fd);
    
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }

    bool delFd(int fd) {
        int fd = sock.GetSockFd();
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }
    
    bool waitFd(std::vector<long long> &list, int ms_timeout = 3000) {
        //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        struct epoll_event evs[10];
        int nfds = epoll_wait(_epfd, evs, 10, ms_timeout);
        if (nfds < 0) {
            perror("epoll wait error");
            return false;
        }else if (nfds == 0) {
            std::cout << "epoll wait timeout\n";
            return false;
        }
        for (int i = 0; i < nfds; i++) {
            int fd = evs[i].data.fd;
            TcpSocket sock;
            sock.SetSockFd(fd);
            list.push_back(sock);
        }
        return true;
    }

    int accept() {
        struct sockaddr_in cli_addr;
        socklen_t len; 
        int newFd;
        while ( (newFd = accept(listener, (SA *) &cli_addr, &len)) != 0) {
            int newFd = accept(listener, (SA *) &cli_addr, &len); 
            if (newFd < 0) { 
                perror("accept"); 
                continue; 
            } else {
                    printf("有连接来自于： %s:%d， 分配的 socket 为:%d/n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), newFd);
            }
            addFd(newFd);
        }
    }

    static void *runTask(void *arg) {
        Epoll *tmp = (Epoll*)arg;
        tmp->comsume();
        return nullptr;
    }
}