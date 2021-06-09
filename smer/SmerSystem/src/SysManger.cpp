#ifdef CLIENT_COMPARE
#include "SysManger.h"
#include <iostream>
#include <functional>
#include <sstream>
using namespace std;


static sysMsgHandle sysMsgHandleTable[] = {
    {SYS_OUTPUT_MSG, bind(&SysManger::handleSysMsgOfShowOutputMsg, &SysManger::getInstance(), placeholders::_1)},
#ifdef CLIENT_COMPARE
    {SYS_SHOW_FRIENDS_MSG, bind(&SysManger::handleSysMsgOfShowFriends, &SysManger::getInstance(), placeholders::_1)},
#endif
    {SYS_SHOW_ASK_FOR_FRIEND_REQ, bind(&SysManger::handleSysMsgOfShowAskForFriendReq, &SysManger::getInstance(), placeholders::_1)}
};

void SysManger::handleSysMsg() {
        shared_ptr<SystemMsgObj> sysObj = nullptr;
        if ((sysObj = SeqToBin::getInstance().tryGetSysMsg()) != nullptr) {
            for (int loop = 0; loop < (sizeof(sysMsgHandleTable) / sizeof(sysMsgHandle)); loop++) {
                if (sysMsgHandleTable[loop].sysMsgType == sysObj->msgType) {
                    sysMsgHandleTable[loop].handler(sysObj);
                }
            }
        }
    }

void SysManger::handleSysMsgOfShowOutputMsg(shared_ptr<SystemMsgObj> sysObj) {
    auto sspt = sysObj->getSysMsgPtr();
    if (sspt == nullptr) {
        return;
    }
    IOManger::getInstance().putOutputMsg(sspt);
}

void SysManger::handleSysMsgOfShowAskForFriendReq(shared_ptr<SystemMsgObj> sysObj) {
    if (sysObj == nullptr) {
        return;
    }
    list<shared_ptr<TransObj>> reqs;
    getAllRequests(reqs);
    auto ss = make_shared<stringstream>();
    (*ss)<<"All requests list down here: \n";
    int i = 0;
    for (auto it = reqs.begin(); it != reqs.end(); it++) {
        (*ss)<<i<<": "<<(*it)->msg<<" \n";
    }
    IOManger::getInstance().putOutputMsg(ss);
}

#ifdef CLIENT_COMPARE
void SysManger::handleSysMsgOfShowFriends(shared_ptr<SystemMsgObj> sysObj)
{
    auto friends = player.getAllFriends();
    auto ss = make_shared<stringstream>();
    int i = 1;
    for (auto it = friends.begin(); it != friends.end(); it++) {
        if (it->second == nullptr) {
            continue;
        }
        (*ss)<<i++<<": "<<it->first<<". ";
        if (i % 3 == 0) {
            (*ss)<<"\n";
        }
    }
    ioManger.putOutputMsg(ss);
}
#endif
#endif