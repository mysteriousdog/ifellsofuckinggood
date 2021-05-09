#ifndef _TALKMANGER_H_
#define _TALKMANGER_H_

#include "Singleton.h"
#include "ConcQueue.h"
#include "SeqAbleObj.h"
#include "SeqToBin.h"
#include <iostream>

class TalkManger : public Singleton<TalkManger>
{
public:
    void operator () () {
        while (true) {
            cout<<"wait for talk..."<<endl;
            TransObj* talkObj = isTalking.pop();
            string str;
            cin>>str;
            if (str.length() >= MAX_TRANS_MSG_LEN) {
                cout<<"talk too much.. your message shoud inside 31 char"<<endl;
                continue;
            }
            memcpy(talkObj->msg, str.c_str(), str.length());
            SeqToBin::getInstance().getBuff().push(talkObj);
        }
    }

private:
    TalkManger(){};
    ConcQueue<TransObj*> isTalking;
friend class Singleton;
};


// class Communicate
// {


// };


#endif // _TALKMANGER_H_