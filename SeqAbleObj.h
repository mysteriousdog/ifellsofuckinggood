#ifndef _SEQABLEOBJ_H_
#define _SEQABLEOBJ_H_
#include <sstream>

class SeqAbleObj
{
public:
    virtual std::stringstream* getSeqData()=0;
    virtual ~SeqAbleObj(){}
};

#endif // _SEQABLEOBJ_H_