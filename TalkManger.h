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
using namespace std;
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
            data_cond.wait(lock, [this] {return this->talking;});
            talking = true;
            struct termios new_settings;
            struct termios stored_settings;
            tcgetattr(0,&stored_settings);
            new_settings = stored_settings;
            new_settings.c_lflag &= (ICANON);
            new_settings.c_lflag &= ECHO;// 关闭回显
            tcsetattr(0,TCSANOW,&new_settings);
            tcsetattr(0,TCSANOW,&stored_settings);
            string str;
            cout<<"Now say someyhing..."<<endl;
            cin>>str;
            getchar();
            cout<<"You say "<<str<<endl;
            if (str.length() >= MAX_TRANS_MSG_LEN) {
                cout<<"talk too much.. your message shoud inside 31 char"<<endl;
                continue;
            }
            TransObj* talkObj = new TransObj(1, MSG_TALK, 0);
            memcpy(talkObj->msg, str.c_str(), str.length());
            SeqToBin::getInstance().getBuff().push(talkObj);
            talking = false;
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

private:
    TalkManger() : talking(false){};
    bool talking;
    mutex data_mutex;
    condition_variable data_cond;
friend class Singleton;
};


// class Communicate
// {


// };


#endif // _TALKMANGER_H_