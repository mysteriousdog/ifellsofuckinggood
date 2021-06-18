#ifndef _INPUT_SYS_MANGER_H_
#define _INPUT_SYS_MANGER_H_
#ifdef CLIENT_COMPARE
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
typedef function<void(shared_ptr<SystemMsgObj>)> SYS_MSG_HANDLE_FUNC;
typedef struct SysMsgHandle {

    SYS_MSG_TYPE_UINT32_ENUM sysMsgType;
    SYS_MSG_HANDLE_FUNC handler;

} sysMsgHandle;

class SysManger : public Singleton<SysManger>
{

public:

    void handleSystem() {
        handleSysMsg();
#ifdef CLIENT_COMPARE
        handleRecvMsg();
        handleSysInput();
#endif
    }

#ifdef CLIENT_COMPARE
    void handleSysInput() {
        Command* cmd = ReadKey::getInstance().getSysCmd();
        if (cmd == nullptr) {
            return;
        }
        switch (cmd->getType())
        {
        case CMD_ENTER:
            cmd->doCommand(ioManger);
            break;
        
        default:
            break;
        }
    }

    void handleRecvMsg() {
        shared_ptr<TransObj> rcvObj;
        if ((rcvObj = SeqToBin::getInstance().getRcvBuff().tryAndPop()) != nullptr) {
            MsgHandler::getInstance().handle(rcvObj, -1);
        }
    }
#endif

    void handleSysMsg();
#ifdef CLIENT_COMPARE
    void handleSysMsgOfShowFriends(shared_ptr<SystemMsgObj> sysObj);
#endif
    void handleSysMsgOfShowAskForFriendReq(shared_ptr<SystemMsgObj> sysObj);
    void handleSysMsgOfShowOutputMsg(shared_ptr<SystemMsgObj> sysObj);

    void pushBackReq(shared_ptr<TransObj> req) {
        reqBuff.push_back(req);
    }
    void pushFrontReq(shared_ptr<TransObj> req) {
        reqBuff.push_front(req);
    }

    shared_ptr<TransObj> popBackReq() {
        return reqBuff.pop_back();
    }
    shared_ptr<TransObj> popFrontReq() {
        return reqBuff.pop_front();
    }
    bool eraseOneReq(int index) {
       return reqBuff.erase(index);
    }
    void getAllRequests(list<shared_ptr<TransObj>>& reqs) {
        reqBuff.getAllData(reqs);
    }
    bool containsReqIdx(int index) {
        if (index >= 0 && index < reqBuff.getSize()) {
            return true;
        }
        return false;
    }
    bool getOneRequest(int index, shared_ptr<TransObj>& req) {
        return reqBuff.getOneRequest(index, req);
    }

private:
    SysManger() : ioManger(IOManger::getInstance()), player(Player::getInstance()){};
    IOManger& ioManger;
    Player& player;
    ConcList<shared_ptr<TransObj>> reqBuff;
    
friend class Singleton;
};




#endif
#endif