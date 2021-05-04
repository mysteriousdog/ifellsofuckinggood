#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <stdint.h>
#include <stdlib.h>
#include "MyEnum.h"
typedef struct TransObj
{
    MSG_TYPE_UINT32_ENUM msgType;
    unsigned int len;
    int id;
    void *msg;
    TransObj():msgType(0),len(0){}
    TransObj(MSG_TYPE_UINT32_ENUM msgType_, unsigned int len_):msgType(msgType_),len(len_){
        msg = malloc(len);
        id = 1;
    }
}tansObj;

class SeqAbleObj
{
public:
    virtual tansObj* getSeqData()=0;
    virtual ~SeqAbleObj(){}
};

#endif // _SEQABLEOBJ_H_