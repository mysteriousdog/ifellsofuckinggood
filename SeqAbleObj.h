#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <stdint.h>
#include <stdlib.h>
#include "MyEnum.h"
#include <iostream>
#include <string.h>
#include <memory>

const int MAX_TRANS_MSG_LEN = 64;


typedef struct TransObj
{
    MSG_TYPE_UINT32_ENUM msgType;
    unsigned int len;
    int id;
    int recverId;
    int fd;
    char msg[MAX_TRANS_MSG_LEN];

    TransObj():msgType(0),len(0), id(0), recverId(0), fd(0){
        memset(msg, 0, MAX_TRANS_MSG_LEN);
    }
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

    bool setMsg(const char* msg_) {
        if (strlen(msg_) >= MAX_TRANS_MSG_LEN) {
            return false;
        }
        clearMsg();
        strncpy(msg, msg_, strlen(msg_));
        return true;
    }
    void clearMsg() {
        memset(msg, 0, MAX_TRANS_MSG_LEN);
    }
    char* getName() {
        return msg;
    }
    char* getMsg() {
        return msg;
    }

    virtual ~TransObj(){std::cout<<"~TransObj() "<<std::endl;}
} tansObj;

typedef struct SystemMsgObj
{
    SYS_MSG_TYPE_UINT32_ENUM msgType;
    std::shared_ptr<std::stringstream> sysMsgPtr;

    SystemMsgObj(SYS_MSG_TYPE_UINT32_ENUM msgType_): msgType(msgType_), sysMsgPtr(nullptr){}
    SystemMsgObj(SYS_MSG_TYPE_UINT32_ENUM msgType_, std::shared_ptr<std::stringstream> sysMsgPtr_): msgType(msgType_),
    sysMsgPtr(sysMsgPtr_){}

    std::shared_ptr<std::stringstream> getSysMsgPtr() {
        return sysMsgPtr;
    }
    void setSysMsgPtr(std::shared_ptr<std::stringstream> sysMsgPtr_) {
        sysMsgPtr = sysMsgPtr_;
    }

}systemMsgObj;

class SeqAbleObj
{
public:
    virtual tansObj* getSeqData()=0;
    virtual ~SeqAbleObj(){}
};

#endif // _SEQABLEOBJ_H_