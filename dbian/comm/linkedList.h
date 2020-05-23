#pragma once

#include "unp.h"
/*
    create by bianbian, 2020/5/20
*/

//双向链表, 非线程安全, 多线程使用要加锁
//数据结构, 为降低分配与清理的性能, 只增加不删除, 标志位清除，重复利用

#define DATALENGTH 2020
#define LISTLEN 50

typedef struct data {
    size_t len;
    char data[DATALENGTH];  //if input size > DATALENGTH, malloc
}dataNode;

typedef struct node {
    struct node *pPrev;
    struct node *pNext;
    dataNode data;
}List, *pNode;


class LinkedList {
private:
    pNode m_head;
    pNode m_middle;
    pNode m_tail;
    size_t m_len;

    pNode& getHead() {
        return m_head;
    }

    pNode& getTail() {
        return m_tail;
    }

    pNode InitNode() {
        pNode node = (pNode)malloc(sizeof(List));
        node->pPrev = nullptr;
        node->pNext = nullptr;
        return node; 
    }

    void setSize(size_t len) {
        m_len = len;
    }

public:
    LinkedList() {
        m_head = nullptr;
        m_middle = nullptr;
        m_tail = nullptr;
        m_len = 0;
    }

    ~LinkedList() {
        freeList();
    }

    size_t getSize() const {
        return m_len;
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
            pNode node = InitNode();
            insertList(node);
        }
    }

    bool insertList(pNode node) {
        pNode p = m_tail;
        if (node == nullptr) {
            node = InitNode();
        }
        node->pNext = nullptr;
        p->pNext = node;
        node->pPrev = p;
        m_tail = node;
        return true;
    }

    bool insertList(const char* str) {
        pNode p = m_head;
        size_t len = strlen(str) > DATALENGTH ? DATALENGTH : strlen(str);
        while (p) {
            if (p->data.len == 0) {
                memcpy(p->data.data, str, len);
                p->data.data[len] = '\0';
                p->data.len = len;
                m_len++;
                return true;
            }
            p = p->pNext;
        }
        pNode node = InitNode();
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
            pNode p = m_head;
            free(p);
            m_head = m_head->pNext;
        }
    }

    const pNode getNode() {
        while (m_middle) {
            if (m_middle->data.len != 0) {
                m_middle->data.len = 0;
                m_len--;
                return m_middle;
            }
            m_middle = m_middle->pNext;
        }
        return nullptr;
    }

    void swapList(LinkedList& llist) {
        pNode p = llist.getHead();
        pNode q = llist.getTail();
        llist.getHead() = m_head;
        llist.getTail() = m_tail;
        m_head = p;
        m_tail = q;
        m_middle = p;   //特有
        m_len = llist.getSize();
        llist.setSize(0);        
    }
};
