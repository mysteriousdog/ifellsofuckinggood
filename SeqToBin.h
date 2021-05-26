#ifndef _SEQTOBIN_H_
#define _SEQTOBIN_H_

#include "Singleton.h"
#include "SeqAbleObj.h"
#include "ConcQueue.h"
#include "IOManger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;
class Client;
// ostream& operator<<(ostream& os,const T& cmd )
// {
// 	//空格分开,方便stream的读入
// 	return os<<user.name<<" "<<user.email<<" "<<user.age;
// }

class SeqToBin : public Singleton<SeqToBin>
{
public:
    void save2Bin(SeqAbleObj& obj)
    {
        tansObj* tansObj = obj.getSeqData();
        buff.push(tansObj);
    }
    ConcQueue<tansObj*>& getBuff() {
        return buff;
    }
    ConcQueue<tansObj*>& getRcvBuff() {
        return rcvBuff;
    }
    SystemMsgObj* tryGetSysMsg() {
        return sysBuff.tryAndPop();
    }
    void putSysMsg(SystemMsgObj* sysObj) {
        sysBuff.push(sysObj);
    }
private:
    ConcQueue<tansObj*> buff;
    ConcQueue<tansObj*> rcvBuff;
    ConcQueue<SystemMsgObj*> sysBuff;
friend class Singleton;
friend class client;
};

#endif