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
    // void save2Bin(SeqAbleObj& obj)
    // {
    //     tansObj* tansObj = obj.getSeqData();
    //     buff.push(tansObj);
    // }
    ConcQueue<shared_ptr<TransObj>>& getBuff() {
        return buff;
    }
    ConcQueue<shared_ptr<TransObj>>& getRcvBuff() {
        return rcvBuff;
    }
    shared_ptr<SystemMsgObj> tryGetSysMsg() {
        return sysBuff.tryAndPop();
    }
    void putSysMsg(shared_ptr<SystemMsgObj> sysObj) {
        sysBuff.push(sysObj);
    }
private:
    ConcQueue<shared_ptr<TransObj>> buff;
    ConcQueue<shared_ptr<TransObj>> rcvBuff;
    ConcQueue<shared_ptr<SystemMsgObj>> sysBuff;
friend class Singleton;
friend class client;
};

#endif