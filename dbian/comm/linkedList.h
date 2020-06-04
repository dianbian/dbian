#pragma once

#include "unp.h"
/*
    create by bianbian, 2020/5/20
*/

//双向链表, 非线程安全, 多线程使用要加锁
//数据结构, 为降低分配与清理的性能, 只增加不删除, 标志位清除，重复利用

#define DATALENGTH 2020
#define LISTLEN 50

template<typename T>
class linkedList {
private:
    T* m_head;
    T* m_middle;
    T* m_tail;
    size_t m_len;
    //TODO 总长度

    void setHead(T* head) {
        m_head = head;
    }

    void setTail(T* tail) {
        m_tail = tail;
    }

    T* InitNode() {
        T *node = (T *)malloc(sizeof(T));
        node->pPrev = nullptr;
        node->pNext = nullptr;
        return node; 
    }

    void setSize(size_t len) {
        m_len = len;
    }

public:
    linkedList() {
        m_head = nullptr;
        m_middle = nullptr;
        m_tail = nullptr;
        m_len = 0;
    }

    ~linkedList() {
        freeList();
    }

    size_t getSize() const {
        return m_len;
    }

    T* getHead() {
        return m_head;
    }
 
    T* getTail() {
        return m_tail;
    }

    void initList(size_t nSize = LISTLEN) {
        if (m_head == nullptr) {    //do not memset
            m_head = InitNode();
            m_tail = InitNode();
            m_middle = m_head;
            m_head->pNext = m_tail;
            m_tail->pPrev = m_head;
            m_tail->pNext = nullptr;
        }
        for (size_t i = 0; i < nSize; ++i) {
            T* node = InitNode();
            insertList(node);
        }
    }

    bool insertList(T* node) {
        T* p = m_tail;
        if (node == nullptr) {
            node = InitNode();
        }
        node->pNext = nullptr;
        p->pNext = node;
        node->pPrev = p;
        m_tail = node;
        return true;
    }

    bool insertList(const char* str) {  //TODO 不够统一DATALENGTH
        T* p = m_head;
        size_t len = strlen(str) > DATALENGTH ? DATALENGTH : strlen(str);
        while (p) {
            if (p->data.len == 0) {
                memcpy(p->data.data, str, len);
                p->data.data[len] = '\0';   //好方法，strlen, sizeof, printf 不用memset
                p->data.len = len;
                m_len++;
                //__sync_fetch_and_add(&m_len, 1);
                return true;
            }
            p = p->pNext;
        }
        T* node = InitNode();
        memcpy(node->data.data, str, len);
        node->data.data[len] = '\0';
        node->data.len = len;
        m_len++;
        return insertList(node);
    }

    void printList() {
        printf("\n\n");
        while (m_head) {
            if (m_head->data.len != 0) {
                printf("lenxxxxx = %ld, %s\n", m_head->data.len, m_head->data.data);
            }
            m_head = m_head->pNext;
        }
    }

    void freeList() {
        printf("\n\n");
        while (m_head) {
            T* p = m_head;
            free(p);
            m_head = m_head->pNext;
        }
    }

    T* getNode() {
        while (m_middle) {
            if (m_middle->data.len != 0) {  //寻找非空对象
                //m_middle->data.len = 0;   //外部调用赋值
                m_len--;
                //__sync_sub_and_fetch(&m_len, 1);
                //m_middle = m_middle->pNext;   //todo
                return m_middle;
            }
            m_middle = m_middle->pNext;
        }
        return nullptr;
    }

    T* getNodeNull() {
        while (m_middle) {
            if (m_middle->data.len == 0) {  //寻找空对象
                m_len++;
                return m_middle;    //TODO 位移
            }
            m_middle = m_middle->pNext;
        }
        T* node = InitNode();
        insertList(node);
        return node;
    }

    void swapList(linkedList& llist) {
        T* p = llist.getHead();
        T* q = llist.getTail();
        llist.setHead(m_head);
        llist.setTail(m_tail);
        m_head = p;
        m_tail = q;
        m_middle = p;   //特有
        m_len = llist.getSize();
        llist.setSize(0);        
    }

    void setMiddle(T* head) {
        m_middle = head;
    }
};
