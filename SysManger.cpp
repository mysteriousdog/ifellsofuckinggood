#include "SysManger.h"
#include <iostream>
#include <functional>
#include <sstream>
using namespace std;

void ii(SystemMsgObj* sysObj){}

static sysMsgHandle sysMsgHandleTable[] = {
    {SYS_SHOW_FRIENDS_MSG, ii},
    {SYS_SHOW_FRIENDS_MSG, bind(&SysManger::handleSysMsgOfShowFriends, &SysManger::getInstance(), placeholders::_1)},
};

void SysManger::handleSysMsgOfShowFriends(SystemMsgObj* sysObj)
{
    auto friends = player.getAllFriends();
    stringstream* ss = new stringstream();
    int i = 1;
    for (auto it = friends.begin(); it != friends.end(); it++) {
        if (it->second == false) {
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
}