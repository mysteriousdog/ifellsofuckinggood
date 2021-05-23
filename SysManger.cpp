#include "SysManger.h"
#include <iostream>
#include <functional>
#include <sstream>
using namespace std;


static sysMsgHandle sysMsgHandleTable[] = {
    {SYS_SHOW_FRIENDS_MSG, bind(&SysManger::handleSysMsgOfShowFriends, &SysManger::getInstance(), placeholders::_1)},
    {SYS_SHOW_ASK_FOR_FRIEND_REQ, bind(&SysManger::handleSysMsgOfShowAskForFriendReq, &SysManger::getInstance(), placeholders::_1)}
};

void SysManger::handleSysMsg() {
        SystemMsgObj* sysObj;
        if (SeqToBin::getInstance().tryGetSysMsg(sysObj)) {
            for (int loop = 0; loop < (sizeof(sysMsgHandleTable) / sizeof(sysMsgHandle)); loop++) {
                if (sysMsgHandleTable[loop].sysMsgType == sysObj->msgType) {
                    sysMsgHandleTable[loop].handler(sysObj);
                }
            }
        }
    }


void SysManger::handleSysMsgOfShowAskForFriendReq(SystemMsgObj* sysObj) {
    if (sysObj == nullptr) {
        return;
    }
    list<TransObj*> reqs;
    getAllRequests(reqs);
    // show requests
    stringstream *ss = new stringstream();
    (*ss)<<"All requests list down here: \n";
    int i = 0;
    for (auto it = reqs.begin(); it != reqs.end(); it++) {
        (*ss)<<i<<": "<<(*it)->msg<<" \n";
    }
    IOManger::getInstance().putOutputMsg(ss);
    delete(sysObj);
}

void SysManger::handleSysMsgOfShowFriends(SystemMsgObj* sysObj)
{
    auto friends = player.getAllFriends();
    stringstream* ss = new stringstream();
    int i = 1;
    for (auto it = friends.begin(); it != friends.end(); it++) {
        if (it->second == nullptr) {
            continue;
        }
        (*ss)<<i++<<": "<<it->first<<". ";
        // (*ss)<<i++;
        // (*ss)<<": ";
        // (*ss)<<it->first;
        // (*ss)<<". ";
        if (i % 3 == 0) {
            (*ss)<<"\n";
        }
    }
   
    cout<<"SysManger::handleSysMsgOfShowFriends "<<ss->str().c_str()<<endl;
    ioManger.putOutputMsg(ss);
     delete(sysObj);
}