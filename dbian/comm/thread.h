#pragma once

#include <pthread.h>

//线程类

//保护共享变量
class guardMutex {
private:
    pthread_mutex_t *m_mutex;   //锁唯一，引用计数不能增加

public:
    guardMutex() = delete;
    explicit guardMutex(pthread_mutex_t *mutex) {
        m_mutex = mutex;
        pthread_mutex_lock(&*m_mutex);
    }
    ~guardMutex() {
        pthread_mutex_unlock(&*m_mutex);
    }
};

class thread {
private:
    pthread_t m_tid;
    char pthreadName[32];
    void *(*m_methodRouter)(void *);
    void *m_args;

public:
    thread(const char *name) {
        if (name == nullptr) {
            struct timeval time;
            gettimeofday(&time, nullptr); /* 获取时间，理论到us */
            snprintf(pthreadName, sizeof(pthreadName), "name_%ld", (time.tv_sec*1000 + time.tv_usec/1000));
            printf("%s\n", pthreadName);
        }
        else {
            memcpy(pthreadName, name, sizeof(pthreadName) > strlen(name) ? strlen(name) : sizeof(pthreadName));
        }
    }

    void setRouter(void *(*methodRouter)(void *), void *args = nullptr) {
        m_methodRouter = methodRouter;
        m_args = args;
    }

    int run() {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    //游离状态
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);     //设置内核级的线程,以获取较高的响应速度
        return pthread_create(&m_tid, &attr, m_methodRouter, m_args);
    }
    
    int getThreadId() {
        return m_tid;   
    }
    void detach() {
        pthread_detach(m_tid);
    }

    void join() {
        pthread_join(m_tid, nullptr);   
    }
};