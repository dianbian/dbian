#pragma once

#include "linkedList.h"
#include "thread.h"

#include <sys/mman.h>
#include <iostream>

class log {
private:
    LinkedList m_produce;
    LinkedList m_comsume;
    std::string m_fileName;
    std::string m_filePath;
    FILE *m_fp;
    int m_fd;
    static log* singel;
    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;  
    pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;  

    log(const log &);
    const log& operator=(const log&);

public:
    log(){  //初始化变量
        pthread_mutex_init(&m_mutex, NULL);
	    pthread_cond_init(&m_cond, NULL);
        m_fp = nullptr;
        m_fd = 0;
    }

    ~log() {
        pthread_mutex_destroy(&m_mutex);
	    pthread_cond_destroy(&m_cond);
    }

    static log* getInstance() { //非线程安全
        if(singel == NULL)
            singel = new log();
        return singel;
    }

    //check parameter
    bool initialize(const char* logName, const char* filePath = "./") {
        m_produce.initList();
        m_comsume.initList();
        m_filePath = filePath;
        int len = strlen(filePath);
        if (filePath[len-1] != '/') {
            m_filePath += '/';
        }
        m_fileName = logName;
        std::string fullName = m_filePath + m_fileName;
        m_fd = ::open(fullName.c_str(), O_CREAT|O_RDWR, 0644);
        if (m_fd < 0) 
            return false;
        m_fp = fdopen(m_fd, "w+");
        if (m_fp == nullptr)
            return false;
        return true;
    }

    void writelog(const char* logContent) {
        int len = strlen(logContent);
        struct stat st;
        fstat(m_fd, &st);
        int fileLen = len + st.st_size;
        ftruncate(m_fd, fileLen);
    	char *addr = (char *)mmap(NULL, fileLen, PROT_WRITE, MAP_SHARED, m_fd, 0); 
        if (addr== MAP_FAILED) {
            fprintf(stderr, "mmap() failed: %s\n", strerror(errno));
            exit(1);
        }
	    memcpy(addr + st.st_size, logContent, len);
        //msync(addr, fileLen, MS_ASYNC); //MS_ASYNC
        msync(addr + st.st_size, len, MS_SYNC); //MS_ASYNC
	    munmap(addr, fileLen);
        return;
    }

    void produce(const char* logContent) {
        guardMutex gm(&m_mutex);
        m_produce.insertList(logContent);
        printf("the thread increase the number:%lu, %lu, pthreadid:%lu\n", m_produce.getSize(), m_comsume.getSize(), (unsigned long)pthread_self());
        if (m_produce.getSize() >= LISTLEN && m_comsume.getSize() == 0) {    //条件
            m_produce.swapList(m_comsume);
            pthread_cond_signal(&m_cond);
        }
        return;
    }

    void comsume() {
        while (1) {
            guardMutex gm(&m_mutex);
            printf("the thread increase the number:%lu, %lu, pthreadid:%lu\n", m_produce.getSize(), m_comsume.getSize(), (unsigned long)pthread_self());
            if (m_comsume.getSize() == 0) {
                pthread_cond_wait(&m_cond, &m_mutex);
            }
            pNode node = m_comsume.getNode();
            if (node == nullptr) {
                sleep(1);
                continue;
            }
            writelog(node->data.data);
            sleep(1);
        }
    }

    static void *runTask(void *arg) {
        log *tmp = (log*)arg;
        tmp->comsume();
        return nullptr;
    }
};

log* log::singel = NULL;    //注意静态变量类外初始化
