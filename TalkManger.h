#ifndef _TALKMANGER_H_
#define _TALKMANGER_H_

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

int const MAX_FRIENDS_NUM = 3000;

class TalkManger : public CommandDoer, public Singleton<TalkManger>
{
public:

    void doCommand(CMD_TYPE_UINT32_ENUM cmdType) override {
        if (cmdType != CMD_ENTER) {
            return;
        }
        turnToTalk();
    }

    void operator () () {
        while (true) {
            cout<<"wait for talk..."<<endl;
            unique_lock<mutex> lock(data_mutex);
            data_cond.wait(lock, [this] {return this->talking || this->getSysMsg;});
            if (this->getSysMsg) {
                handleSysMsg();
            } else if (this->talking) {
                handleTalk();
            }
            
        }
    }
    void turnToTalk() {
        unique_lock<mutex> lock(data_mutex);
        std::cout<<"now turn to talking !!"<<std::endl;
        // TransObj* obj = new TransObj(1, MSG_TALK, 0);
        // talkingQueue.push(obj);
        talking = true;
        data_cond.notify_one();
    }
    bool isTalking() {
        unique_lock<mutex> lock(data_mutex);
        return talking;
    }

    string& getTalkerName() {
        return talkerName;
    }
    void setTalkerName(string&& name) {
        talkerName = name;
    }

    map<string, bool>& getAllFriends() {
        return friends;
    } 

    bool addFriend(string&& name) {
        if (friends.size() >= MAX_FRIENDS_NUM) {
            return false;
        }
        friends[name] = true;
        isFriendsChanged = true;
        return true;
    }
    bool delFriend(string&& name) {
        if (friends.count(name) > 0) {
            friends[name] = false;
            isFriendsChanged = true;
            return true;
        }
        return false;
    }

    void handleSysMsg();
    void handleTalk();

private:
    TalkManger() : talking(false), isFriendsChanged(false), getSysMsg(false){
        // load friends
    };
    bool talking;
    bool getSysMsg;
    bool isFriendsChanged;
    mutex data_mutex;
    condition_variable data_cond;
    string talkerName;
    map<string, bool> friends;
friend class Singleton;
};


// class Communicate
// {


// };


#endif // _TALKMANGER_H_