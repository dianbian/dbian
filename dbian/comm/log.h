#pragma once

#include "linkedList.h"
#include "thread.h"

#include <sys/mman.h>
#include <iostream>

class log{
private:
    LinkedList m_produce;
    LinkedList m_comsume;
    std::string m_fileName;
    std::string m_filePath;
    FILE *m_fp;
    int m_fd;

    log(const log &);
    const log& operator=(const log&);

public:
    log() {

    }

    //check parameter
    bool initialize(const char* logName, const char* filePath = "./") {
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
    }
};
