#ifndef _SEQTOBIN_H_
#define _SEQTOBIN_H_

#include "Singleton.h"
#include "SeqAbleObj.h"
#include "ConcQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

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
        stringstream* ss = obj.getSeqData();
        string s = ss->str();
        buff.push(s);
        delete ss;
    }
private:
    ConcQueue<string> buff;
friend class Singleton;
friend class client;
};

#endif