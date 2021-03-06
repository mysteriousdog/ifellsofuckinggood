#ifndef _IOManger_H_
#define _IOManger_H_
#ifdef CLIENT_COMPARE
#include "Log.h"
#include "Singleton.h"
#include "ConcQueue.h"
#include "SeqAbleObj.h"
#include "SeqToBin.h"
#include <iostream>
#include <termios.h>
#include <mutex>
#include <condition_variable>
#include "CommandDoer.h"
#include "Player.h"
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

    void run() {
        LOG_DEBUG("IOManger running");
        while (true) {
            unique_lock<mutex> lock(data_mutex);
            data_cond.wait(lock, [this] {return this->talking || !(this->outputMsg.empty());});
            if (!this->talking) {
                handleOutputMsg();
            } else {
                handleTalk();
            }
            
        }
    }

    void operator () () {
        run();
    }
    bool tryLoginFirst();
    bool isTalking() {
        unique_lock<mutex> lock(data_mutex);
        return talking;
    }

    void turnToTalk() {
        unique_lock<mutex> lock(data_mutex);
        talking = true;
        data_cond.notify_one();
    }

    void putOutputMsg(std::shared_ptr<std::stringstream> msg) {
        unique_lock<mutex> lock(data_mutex);
        outputMsg.push(msg);
        data_cond.notify_one();
    }

    void handleOutputMsg();
    void handleTalk();

private:
    IOManger() : talking(false) {
    };
    bool talking;
    ConcQueue<std::shared_ptr<std::stringstream> > outputMsg;
    mutex data_mutex;
    condition_variable data_cond;
    
friend class Singleton;
};


#endif // _IOManger_H_
#endif