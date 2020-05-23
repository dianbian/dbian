#pragma once

#include <sys/epoll.h>

class Epoll {
private:
    int m_epfd;

public:
    bool Init() {
        //创建epoll
        
        //接口原型：int epoll_create(int size);
        _epfd = epoll_create(1);
        if (_epfd < 0) {
            perror("epoll create error");
            return false;
        }
        return true;
    }

    bool Add(TcpSocket sock, uint32_t events = 0) {
        sock.SetNonBlock();
        int fd = sock.GetSockFd();
        
        //定义事件
        struct epoll_event ev;
        ev.events = EPOLLIN | events;
        ev.data.fd = fd;
        
        //epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }

    bool Del(TcpSocket sock) {
        int fd = sock.GetSockFd();
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
        if (ret < 0) {
            perror("epoll ctrl error");
            return false;
        }
        return true;
    }
    
    bool Wait(std::vector<TcpSocket> &list, int ms_timeout = 3000) {
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
}