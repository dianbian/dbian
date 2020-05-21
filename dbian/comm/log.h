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
        printf("%d, %p\n", m_fd, m_fp);
        return true;
    }

    void writelog(const char* logContent) {
        int len = strlen(logContent);
        //ftruncate(m_fd, len);
        int length = ::lseek(m_fd, len-1, SEEK_END);
        ::write(m_fd, "", 1);
    	char *addr = (char *)mmap(NULL, length + len, PROT_WRITE, MAP_SHARED, m_fd, 0); 
        if (addr== MAP_FAILED) {
            fprintf(stderr, "mmap() failed: %s\n", strerror(errno));
            exit(1);
        }
	    memcpy(addr + length, logContent, len);
	    munmap(addr + length, len);
    }
};
