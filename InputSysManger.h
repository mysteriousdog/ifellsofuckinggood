#ifndef _INPUT_SYS_MANGER_H_
#define _INPUT_SYS_MANGER_H_

#include "Singleton.h"
#include "TalkManger.h"
#include "ReadKey.h"
#include "Command.h"
#include <iostream>
using namespace std;

class InputSysManger : public Singleton<InputSysManger>
{

public:
    void handleSysInput() {
        Command* cmd = ReadKey::getInstance().getSysCmd();
        if (cmd == nullptr) {
            return;
        }
        switch (cmd->getType())
        {
        case CMD_ENTER:
            cout<<"InputSysManger CMD_ENTER!!"<<endl;
            cmd->doCommand(talkManger);
            break;
        
        default:
            break;
        }
    }
    void handleSysRecvMsg() {
        TransObj* rcvObj;
        if (SeqToBin::getInstance().getRcvBuff().tryAndPop(rcvObj)) {
            MsgHandler::getInstance().handle(rcvObj, -1);
        }
    }

private:
    InputSysManger() : talkManger(TalkManger::getInstance()){};
    TalkManger& talkManger;
friend class Singleton;
};

#endif