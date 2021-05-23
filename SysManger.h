#ifndef _INPUT_SYS_MANGER_H_
#define _INPUT_SYS_MANGER_H_

#include "Singleton.h"
#include "IOManger.h"
#include "ReadKey.h"
#include "Command.h"
#include "HandleMsg.h"
#include "ThreadPool.h"
#include "SeqToBin.h"
#include "Player.h"
#include "ConcList.h"
#include <iostream>
#include <list>
using namespace std;


// typedef void (*SYS_MSG_HANDLE_FUNC_PTR)(SystemMsgObj* sysObj);
typedef function<void(SystemMsgObj*)> SYS_MSG_HANDLE_FUNC;
typedef struct SysMsgHandle {

    SYS_MSG_TYPE_UINT32_ENUM sysMsgType;
    SYS_MSG_HANDLE_FUNC handler;

} sysMsgHandle;

class SysManger : public Singleton<SysManger>
{

public:

    void handleSystem() {
        handleSysMsg();
        handleRecvMsg();
        handleSysInput();
    }

    void handleSysInput() {
        Command* cmd = ReadKey::getInstance().getSysCmd();
        if (cmd == nullptr) {
            return;
        }
        switch (cmd->getType())
        {
        case CMD_ENTER:
            cout<<"SysManger CMD_ENTER!!"<<endl;
            cmd->doCommand(ioManger);
            break;
        
        default:
            break;
        }
    }
    void handleRecvMsg() {
        TransObj* rcvObj;
        if (SeqToBin::getInstance().getRcvBuff().tryAndPop(rcvObj)) {
            MsgHandler::getInstance().handle(rcvObj, -1);
        }
    }

    void handleSysMsg();

    void handleSysMsgOfShowFriends(SystemMsgObj* sysObj);
    void handleSysMsgOfShowAskForFriendReq(SystemMsgObj* sysObj);

    void pushBackReq(requestObj* req) {
        reqBuff.push_back(req);
    }
    void pushFrontReq(requestObj* req) {
        reqBuff.push_front(req);
    }

    requestObj* popBackReq() {
        reqBuff.pop_back();
    }
    requestObj* popFrontReq() {
        reqBuff.pop_front();
    }
    bool eraseOneReq(int index) {
       return reqBuff.erase(index);
    }
    void getAllRequests(list<requestObj*> reqs) {
        reqBuff.getAllData(reqs);
    }
    bool containsReqIdx(int index) {
        if (index >= 0 && index < reqBuff.getSize()) {
            return true;
        }
        return false;
    }

private:
    SysManger() : ioManger(IOManger::getInstance()), player(Player::getInstance()){};
    IOManger& ioManger;
    Player& player;
    ConcList<requestObj*> reqBuff;
    
friend class Singleton;
};




#endif