#ifndef _IOManger_H_
#define _IOManger_H_

#include "Singleton.h"
#include "ConcQueue.h"
#include "SeqAbleObj.h"
#include "SeqToBin.h"
#include <iostream>
#include <termios.h>
#include <mutex>
#include <condition_variable>
#include "CommandDoer.h"
#include <string.h>
#include <list>
#include <map>
using namespace std;

class IOManger : public CommandDoer, public Singleton<IOManger>
{
public:

    void doCommand(CMD_TYPE_UINT32_ENUM cmdType) override {
        if (cmdType == CMD_ENTER) {
            turnToTalk();
            
        }
        return;
        
    }

    void operator () () {
        while (true) {
            cout<<"wait for talk..."<<endl;
            unique_lock<mutex> lock(data_mutex);
            data_cond.wait(lock, [this] {return this->talking || this->outputMsg.waitTillNotEmpty();});
            if (!this->talking) {
                handleOutputMsg();
            } else {
                handleTalk();
            }
            
        }
    }

    bool isTalking() {
        unique_lock<mutex> lock(data_mutex);
        return talking;
    }

    void turnToTalk() {
        unique_lock<mutex> lock(data_mutex);
        std::cout<<"now turn to talking !!"<<std::endl;
        // TransObj* obj = new TransObj(1, MSG_TALK, 0);
        // talkingQueue.push(obj);
        talking = true;
        data_cond.notify_one();
    }

    void putOutputMsg(stringstream* msg) {
        std::cout<<"now turn to show msg !!"<<std::endl;
        // TransObj* obj = new TransObj(1, MSG_TALK, 0);
        // talkingQueue.push(obj);
        outputMsg.push(msg);
    }

    void handleOutputMsg();
    void handleTalk();

private:
    IOManger() : talking(false){
        // load friends
    };
    bool talking;
    ConcQueue<stringstream*> outputMsg;
    mutex data_mutex;
    condition_variable data_cond;
    
friend class Singleton;
};


// class Communicate
// {


// };


#endif // _IOManger_H_