#ifdef CLIENT_COMPARE
#include "Util.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include<boost/format.hpp>
#include <iostream>
#include "Player.h"
#include "IOManger.h"
#include "ReadKey.h"
#include <list>
#include <stdlib.h>
#include "SysManger.h"

static CommunicationCmd communicationCmdTable[] = {
    {"li", handleInputOfLogin},
    {"login", handleInputOfLogin},
    {"Login", handleInputOfLogin},
    {"LOGIN", handleInputOfLogin},
    {"lo", handleInputOfLogout},
    {"logout", handleInputOfLogout},
    {"Logout", handleInputOfLogout},
    {"LOGOUT", handleInputOfLogout},
    {"rg", handleInputOfRegin},
    {"regin", handleInputOfRegin},
    {"Regin", handleInputOfRegin},
    {"REGIN", handleInputOfRegin},
    {"ad", handleInputOfAskForFriend},
    {"add", handleInputOfAskForFriend},
    {"Add", handleInputOfAskForFriend},
    {"ch", handleInputOfChoseTalker},
    {"chose", handleInputOfChoseTalker},
    {"Chose", handleInputOfChoseTalker},
    {"CHOSE", handleInputOfChoseTalker},
    {"sf", handleInputOfShowFriends},
    {"showfriends", handleInputOfShowFriends},
    {"showFriends", handleInputOfShowFriends},
    {"ShowFriends", handleInputOfShowFriends},
    {"sr", handleInputOfShowRequests},
    {"showrequests", handleInputOfShowRequests},
    {"showRequests", handleInputOfShowRequests},
    {"ShowRequests", handleInputOfShowRequests},
    {"SHOWREQUESTS", handleInputOfShowRequests},
    {"ar", handleInputOfAccRequest},
    {"acceptrequest", handleInputOfAccRequest},
    {"acceptRequest", handleInputOfAccRequest},
    {"Acceptrequest", handleInputOfAccRequest},
    {"ACCEPTREQUEST", handleInputOfAccRequest},
    {"Me", handleInputOfShowMySelf},
    {"me", handleInputOfShowMySelf},
};

bool splitInputStr2NameAndPwd(vector<string>& res, string&& input) {
    Util::getInstance().splitStr(res, move(input), "|");
    if (res.size() <= 1 || res.size() > 2 || res[0].length() > NAME_MAX_LEN - 1 || res[1].length() > PASSWORD_MAX_LEN - 1) {
        return false;
    }
    return true;
}

shared_ptr<TransObj> handleInputOfLogin(string&& input) {
    vector<string> res;
    if (!splitInputStr2NameAndPwd(res, move(input))) {
        return nullptr;
    }
    auto obj = make_shared<TransObj>(-1, -1, MSG_LOGIN, MAX_TRANS_MSG_LEN, -1);
    if(obj->setNamePasswd(res[0].c_str(), res[1].c_str())) {
        return obj;
    }
    delete(obj);
    LOG_ERR("handleInputOfLogin setNamePasswd err!");
    return nullptr;
}

shared_ptr<TransObj> handleInputOfLogout(string&& input) {
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    auto obj = make_shared<TransObj>(-1, -1, MSG_LOGOUT, -1, -1);
    obj->setMsgType(MSG_LOGOUT);
    return obj;
}

shared_ptr<TransObj> handleInputOfRegin(string&& input) {
    vector<string> res;
    if (!splitInputStr2NameAndPwd(res, move(input))) {
        return nullptr;
    }
    auto obj = make_shared<TransObj>(-1, -1, MSG_REG, MAX_TRANS_MSG_LEN, -1);
    if(obj->setNamePasswd(res[0].c_str(), res[1].c_str())) {
        return obj;
    }
    delete(obj);
    return nullptr;
}

shared_ptr<TransObj> handleInputOfAskForFriend(string&& input) {
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    if (input.length() >= NAME_MAX_LEN) {
        LOG_INFO("name length should be in " + to_string(NAME_MAX_LEN - 1));
        return nullptr;
    }
    auto obj = make_shared<TransObj>(-1, -1, MSG_ASK_FOR_FRIEND, MAX_TRANS_MSG_LEN, -1);
    obj->setMsg(input.c_str());
    return obj;
}

shared_ptr<TransObj> handleInputOfChoseTalker(string&& input) {
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    if (input.length() >= NAME_MAX_LEN) {
        LOG_INFO("name length should be in " + to_string(NAME_MAX_LEN - 1));
        return nullptr;
    }
    // 首先要判断这个人存不存在 然后判断是不是好友 最后判断是不是在线
    Player::getInstance().setTalkerName(move(input));
    auto obj = make_shared<TransObj>(-1, -1, MSG_ASK_FOR_FRIEND, MAX_TRANS_MSG_LEN, -1);
    obj->setMsg(input.c_str());
    return obj;
}

shared_ptr<TransObj> handleInputOfShowFriends(string&& input) {
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    SystemMsgObj* sysObj = new SystemMsgObj(SYS_SHOW_FRIENDS_MSG);
    SeqToBin::getInstance().putSysMsg(sysObj);
    // show 所有的friends
    return nullptr;
}

shared_ptr<TransObj> handleInputOfShowRequests(string&& input)
{
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    // SystemMsgObj* sysObj = new SystemMsgObj(SYS_SHOW_FRIENDS_MSG);
    // SeqToBin::getInstance().putSysMsg(sysObj);
    // show 所有的friends
    // list<RequestObj*> allReqs;
    // SysManger::getInstance().getAllRequests(allReqs);
    SystemMsgObj* sysObj = new SystemMsgObj(SYS_SHOW_ASK_FOR_FRIEND_REQ);
    SeqToBin::getInstance().putSysMsg(sysObj);
    
    return nullptr;
}

shared_ptr<TransObj> handleInputOfAccRequest(string&& input)
{
    if (IOManger::getInstance().tryLoginFirst()) {
        return nullptr;
    }
    int index = atoi(input.c_str());
    if (SysManger::getInstance().containsReqIdx(index)) {
        shared_ptr<TransObj> reqObj = nullptr;
        if (!SysManger::getInstance().getOneRequest(index, reqObj)) {
            shared_ptr<stringstream> ss = make_shared<stringstream>();
            (*ss)<<"Your command of accept the request send err"<<" \n";
            SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
            SeqToBin::getInstance().putSysMsg(sysObj);
            return nullptr;
        }
        int recvId = reqObj->id;
        string name = reqObj->msg;
        auto obj = make_shared<TransObj>(Player::getInstance().getPlayerId(), recvId, MSG_ASK_FOR_FRIEND_ACCEPT, MAX_TRANS_MSG_LEN, -1);
        shared_ptr<stringstream> ss = make_shared<stringstream>();
        (*ss)<<"Your command of accept the request have been send"<<" \n";
        SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
        SeqToBin::getInstance().putSysMsg(sysObj);
        Player::getInstance().addFriend(move(name), recvId);
        return obj;
    }
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"Your input number of accepted request id not exists ==> "<<input<<" \n";
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    return nullptr;
}

void Util::splitStr(vector<string>& res, string&& source, string&& fg)
{
    boost::split(res, source, boost::is_any_of(fg));
}

shared_ptr<TransObj> Util::getMsgFromInput(string&& input)
{
    vector<string> res;
    splitStr(res, move(input), INPUTSTR_SPLIT_FLAG);
    int n = res.size();
    if (n > 2 || n < 1) { 
        return nullptr;
    }
    
    if (n == 1) { // 此时 是 talk
        if (IOManger::getInstance().tryLoginFirst()) {
            return nullptr;
        }
        if (input.length() >= MAX_TRANS_MSG_LEN || input.length() <= 0) {
            LOG_INFO("name length should be in 1 ~ " + to_string(MAX_TRANS_MSG_LEN));
        }
        auto obj = make_shared<TransObj>(1, 1, MSG_TALK, 1, 1);
        obj->setMsg(input.c_str());
        // 设置talk对象 recverId_
        return obj;
    }
    string cmdType = res[0];
    string cmd = res[1];

    for (int loop = 0; loop < (sizeof(communicationCmdTable) / sizeof(CommunicationCmd)); loop++) {
        if (cmdType == communicationCmdTable[loop].comtype) {
            return communicationCmdTable[loop].handler(move(cmd));
        }
    }

    return nullptr;
}


shared_ptr<TransObj> handleInputOfShowMySelf(string&& input)
{
    int id = Player::getInstance().getPlayerId();
    string name = Player::getInstance().getPlayerName();
    string passwd = Player::getInstance().getPlayerPwd();
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"Your info: "<<" \n";
    (*ss)<<"id: "<<id<<" \n";
    (*ss)<<"name: "<<name<<" \n";
    (*ss)<<"passwd: "<<passwd<<" \n";
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    return nullptr;
}

#endif

