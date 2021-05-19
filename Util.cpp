#include "Util.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include<boost/format.hpp>
#include <iostream>
#include "Player.h"
#include "TalkManger.h"

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
    {"showfriends", handleInputOfShowFriends},
    {"showFriends", handleInputOfShowFriends},
    {"ShowFriends", handleInputOfShowFriends},
    {"sf", handleInputOfShowFriends},
};

bool splitInputStr2NameAndPwd(vector<string>& res, string&& input) {
    Util::getInstance().splitStr(res, move(input), "|");
    if (res.size() <= 1 || res.size() > 2 || res[0].length() > NAME_MAX_LEN - 1 || res[1].length() > PASSWORD_MAX_LEN - 1) {
        std::cout<<"name length should be in "<<NAME_MAX_LEN - 1<<endl;
        std::cout<<"passwd length should be in "<<PASSWORD_MAX_LEN<<endl;
        return false;
    }
    return true;
}

TransObj* handleInputOfLogin(string&& input) {
    vector<string> res;
    if (!splitInputStr2NameAndPwd(res, move(input))) {
        return nullptr;
    }
    TransObj* obj = new TransObj(-1, -1, MSG_LOGIN, MAX_TRANS_MSG_LEN, -1);
    snprintf(obj->msg, NAME_MAX_LEN, res[0].c_str());
    snprintf(obj->msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, res[1].c_str());
    return obj;
}

TransObj* handleInputOfLogout(string&& input) {
    TransObj* obj = new TransObj(-1, -1, MSG_LOGOUT, -1, -1);
    obj->setMsgType(MSG_LOGOUT);
    return obj;
}

TransObj* handleInputOfRegin(string&& input) {
    vector<string> res;
    if (!splitInputStr2NameAndPwd(res, move(input))) {
        return nullptr;
    }
    TransObj* obj = new TransObj(-1, -1, MSG_REG, MAX_TRANS_MSG_LEN, -1);
    snprintf(obj->msg, NAME_MAX_LEN, res[0].c_str());
    snprintf(obj->msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, res[1].c_str());
    return obj;
}

TransObj* handleInputOfAskForFriend(string&& input) {
    if (input.length() >= NAME_MAX_LEN) {
        cout<<"name length should be in "<<NAME_MAX_LEN - 1<<endl;
        return nullptr;
    }
    TransObj* obj = new TransObj(-1, -1, MSG_ASK_FOR_FRIEND, MAX_TRANS_MSG_LEN, -1);
    snprintf(obj->msg, NAME_MAX_LEN, input.c_str());
    return obj;
}

TransObj* handleInputOfChoseTalker(string&& input) {
    if (input.length() >= NAME_MAX_LEN) {
        cout<<"name length should be in "<<NAME_MAX_LEN - 1<<endl;
        return nullptr;
    }
    // 首先要判断这个人存不存在 然后判断是不是好友 最后判断是不是在线
    TalkManger::getInstance().setTalkerName(move(input));
    TransObj* obj = new TransObj(-1, -1, MSG_ASK_FOR_FRIEND, MAX_TRANS_MSG_LEN, -1);
    snprintf(obj->msg, NAME_MAX_LEN, input.c_str());
    return obj;
}

TransObj* handleInputOfShowFriends(string&& input) {
    auto friends = TalkManger::getInstance().getAllFriends();
    // show 所有的friends
    return nullptr;
}

void Util::splitStr(vector<string>& res, string&& source, string&& fg)
{
    boost::split(res, source, boost::is_any_of(fg));
}

TransObj* Util::getMsgFromInput(string&& input)
{
    vector<string> res;
    splitStr(res, move(input), INPUTSTR_SPLIT_FLAG);
    int n = res.size();
    if (n > 2 || n < 1) { 
        return nullptr;
    }
    
    if (n == 1) { // 此时 是 talk
        if (input.length() >= MAX_TRANS_MSG_LEN || input.length() <= 0) {
            std::cout<<"msg length should be in 1 ~  "<<MAX_TRANS_MSG_LEN<<endl;
        }
        TransObj* obj = new TransObj(1, 1, MSG_TALK, 1, 1);
        snprintf(obj->msg, MAX_TRANS_MSG_LEN, input.c_str());
        // 设置talk对象 recverId_
        return obj;
    }
    string cmdType = res[0];
    string cmd = res[1];

    for (int loop = 0; loop < (sizeof(communicationCmdTable) / sizeof(CommunicationCmd)); loop++) {
        cout<<"cmdType is "<<cmdType<<" communicationCmdTable now is "<<communicationCmdTable[loop].comtype<<endl;
        if (cmdType == communicationCmdTable[loop].comtype) {
            return communicationCmdTable[loop].handler(move(cmd));
        }
    }

    return nullptr;
}