#pragma once
/*
    create by bianbian, 2020/5/29s
*/

typedef struct {
    unsigned short msgType; //65535
    unsigned int msgLen;    //4294967295
} msgHeader;

#define HEADLEN sizeof(msgHeader)

enum {
    REQUESE1 = 0x5321,
    REQUESE2,
    REQUESE3,
    REQUESE4,
    REQUESE5,
    REQUESE6,
};