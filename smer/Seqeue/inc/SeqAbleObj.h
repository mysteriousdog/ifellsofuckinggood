#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <stdint.h>
#include <stdlib.h>
#include "MyEnum.h"
#include <iostream>
#include <string.h>
#include <memory>
#include <string.h>
#include "Player.h"

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
    int getrecverId() {
        return recverId;
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

    bool setName(string& name_) {
        if (name_.length() >= NAME_MAX_LEN) {
            return false;
        }
        clearName();
        // strncpy(msg, name_.c_str(), NAME_MAX_LEN);
        snprintf(msg, NAME_MAX_LEN, name_.c_str());
        return true;
    }

    bool setName(const char* name_) {
        if (strlen(name_)>= NAME_MAX_LEN) {
            return false;
        }
        clearName();
        // strncpy(msg, name_, NAME_MAX_LEN);
        snprintf(msg, NAME_MAX_LEN, name_);
        return true;
    }

    bool setPasswd(string& passwd_) {
        if (passwd_.length() >= PASSWORD_MAX_LEN) {
            return false;
        }
        clearPasswd();
        // strncpy(msg + NAME_MAX_LEN, passwd_.c_str(), PASSWORD_MAX_LEN);
        snprintf(msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, passwd_.c_str());
        return true;
    }

    bool setPasswd(const char* passwd_) {
        if (strlen(passwd_) >= PASSWORD_MAX_LEN) {
            return false;
        }
        clearPasswd();
        // strncpy(msg + NAME_MAX_LEN, passwd_, PASSWORD_MAX_LEN);
        snprintf(msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, passwd_);
        return true;
    }

    bool setNamePasswd(const char* name_, const char* passwd_) {
        if (strlen(name_) >= NAME_MAX_LEN || strlen(passwd_) >= PASSWORD_MAX_LEN) {
            return false;
        }
        clearMsg();
        setName(name_);
        setPasswd(passwd_);
        // strncpy(msg, name_, NAME_MAX_LEN);
        // strncpy(msg + NAME_MAX_LEN, passwd_, PASSWORD_MAX_LEN);
        return true;
    }

    bool transBuff2Obj(char* buff) {
        if (buff == nullptr) {
            return false;
        }
        this->setrecverId(((TransObj*)buff)->getrecverId());
        this->setFd(((TransObj*)buff)->getFd());
        this->setId(((TransObj*)buff)->getId());
        this->setMsgType(((TransObj*)buff)->getMsgType());
        this->setMsg(((TransObj*)buff)->getMsg());
        memcpy(msg, (const char*)(((TransObj*)buff)->getMsg()), MAX_TRANS_MSG_LEN);
        return true;
    }

    void clearMsg() {
        memset(msg, 0, MAX_TRANS_MSG_LEN);
    }
    void clearName() {
        memset(msg, 0 , NAME_MAX_LEN);
    }
    void clearPasswd() {
        memset(msg + NAME_MAX_LEN, 0 , PASSWORD_MAX_LEN);
    }
    const char* getName() {
        return msg;
    }
    const char* getPasswd() {
        return msg + NAME_MAX_LEN;
    }
    const char* getMsg() {
        return msg;
    }

    int getId() {
        return id;
    }

    int getFd() {
        return fd;
    }

    ~TransObj(){}
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