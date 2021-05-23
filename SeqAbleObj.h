#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <stdint.h>
#include <stdlib.h>
#include "MyEnum.h"
#include <iostream>
#include <string.h>

const int MAX_TRANS_MSG_LEN = 64;


typedef struct TransObj
{
    MSG_TYPE_UINT32_ENUM msgType;
    unsigned int len;
    int id;
    int recverId;
    int fd;
    char msg[MAX_TRANS_MSG_LEN];
    TransObj():msgType(0),len(0){}
    TransObj(MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_):msgType(msgType_),len(len_){
        id = 1;
    }
    TransObj(int id_, MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_):id(id_),msgType(msgType_),len(len_){}
    TransObj(int id_, MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_, int fd_):id(id_),msgType(msgType_),len(len_), fd(fd_){}
    TransObj(int id_, int recverId_ ,MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_, int fd_):
    msgType(msgType_),
    len(len_),
    id(id_),
    recverId(recverId_),
    fd(fd_)
    {}

    void setMsgType(MSG_TYPE_UINT32_ENUM msgType_) {
        msgType = msgType_;
    }
    MSG_TYPE_UINT32_ENUM getMsgType() {
        return msgType;
    }
    void setId(int id_) {
        id = id_;
    }
    void setFd(int fd_) {
        fd = fd_;
    }
    void setrecverId(int recverId_) {
        recverId = recverId_;
    }
    void setLen(int len_) {
        len = len_;
    }

    void setMsg(char* msg_) {
        strncpy(msg, msg_, strlen(msg_));
    }
    char* getName() {
        return msg;
    }

    virtual ~TransObj(){}
} tansObj;

typedef struct SystemMsgObj
{
    SYS_MSG_TYPE_UINT32_ENUM msgType;

    SystemMsgObj(SYS_MSG_TYPE_UINT32_ENUM msgType_): msgType(msgType_){}

}systemMsgObj;

class SeqAbleObj
{
public:
    virtual tansObj* getSeqData()=0;
    virtual ~SeqAbleObj(){}
};

#endif // _SEQABLEOBJ_H_