#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <stdint.h>
#include <stdlib.h>
#include "MyEnum.h"



const int MAX_TRANS_MSG_LEN = 32;

typedef struct TransObj
{
    MSG_TYPE_UINT32_ENUM msgType;
    unsigned int len;
    int id;
    int fd;
    char msg[MAX_TRANS_MSG_LEN];
    TransObj():msgType(0),len(0){}
    TransObj(MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_):msgType(msgType_),len(len_){
        id = 1;
    }
    TransObj(int id_, MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_):id(id_),msgType(msgType_),len(len_){}
    TransObj(int id_, MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_, int fd_):id(id_),msgType(msgType_),len(len_), fd(fd_){}
    void setMsgType(MSG_TYPE_UINT32_ENUM msgType_) {
        msgType = msgType_;
    }
    void setId(int id_) {
        id = id_;
    }
} tansObj;

class SeqAbleObj
{
public:
    virtual tansObj* getSeqData()=0;
    virtual ~SeqAbleObj(){}
};

#endif // _SEQABLEOBJ_H_