#include "Util.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include<boost/format.hpp>
#include <iostream>
#include "Player.h"

static CommunicationCmd communicationCmdTable[] = {
    {"login", nullptr}
};

TransObj* handleInputOfLogin(string&& input) {
    vector<string> res;
    splitStr(res, move(input), "|");
    if (res.size() <= 1 || res.size() > 2 || res[0].length() > NAME_MAX_LEN - 1 || res[1].length() > PASSWORD_MAX_LEN - 1) {
        std::cout<<"login cmd shoud be split with |"<<endl;
        std::cout<<"name length should be in "<<NAME_MAX_LEN<<endl;
        std::cout<<"passwd length should be in "<<PASSWORD_MAX_LEN<<endl;
        return nullptr;
    }
    TransObj* obj = new TransObj(-1, -1, MSG_LOGIN, MAX_TRANS_MSG_LEN, -1);
    snprintf(obj->msg, NAME_MAX_LEN, res[0].c_str());
    snprintf(obj->msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, res[1].c_str());
    return obj;
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
        // 设置talk对象 recverId_
        return obj;
    }
    string cmdType = res[0];
    string cmd = res[1];
    if (cmdType == "login" || cmdType == "LOGIN") {
        splitStr(res, move(res[1]), "|");
        if (res.size() <= 1 || res.size() > 2 || res[0].length() > NAME_MAX_LEN - 1 || res[1].length() > PASSWORD_MAX_LEN - 1) {
            std::cout<<"login cmd shoud be split with |"<<endl;
            std::cout<<"name length should be in "<<NAME_MAX_LEN<<endl;
            std::cout<<"passwd length should be in "<<PASSWORD_MAX_LEN<<endl;
            return nullptr;
        }
        TransObj* obj = new TransObj(-1, -1, MSG_LOGIN, MAX_TRANS_MSG_LEN, -1);
        snprintf(obj->msg, NAME_MAX_LEN, res[0].c_str());
        snprintf(obj->msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, res[1].c_str());
        return obj;
        // 设置name password
    } else if (cmdType == "logout" || cmdType == "LOGOUT") {
        TransObj* obj = new TransObj(-1, -1, MSG_LOGOUT, -1, -1);
        obj->setMsgType(MSG_LOGOUT);
        return obj;
        
    } else if (cmdType == "regin" || cmdType == "REGIN") {
        splitStr(res, move(res[1]), "|");
        if (res.size() <= 1 || res.size() > 2 || res[0].length() > NAME_MAX_LEN - 1 || res[1].length() > PASSWORD_MAX_LEN - 1) {
            std::cout<<"regin cmd shoud be split with |"<<endl;
            std::cout<<"name length should be in "<<NAME_MAX_LEN<<endl;
            std::cout<<"passwd length should be in "<<PASSWORD_MAX_LEN<<endl;
            return nullptr;
        }
        TransObj* obj = new TransObj(-1, -1, MSG_LOGIN, MAX_TRANS_MSG_LEN, -1);
        obj->setMsgType(MSG_REG);
        snprintf(obj->msg, NAME_MAX_LEN, res[0].c_str());
        snprintf(obj->msg + NAME_MAX_LEN, PASSWORD_MAX_LEN, res[1].c_str());
        return obj;
        // 设置name password
    } else if (cmdType == "add" || cmdType == "ADD") {
        TransObj* obj = new TransObj(1, 1, MSG_LOGIN, 1, 1);
        obj->setMsgType(MSG_ASK_FOR_FRIEND);
        return obj;
        // 设置name password
    } else {
        //  后续
        TransObj* obj = new TransObj(1, 1, MSG_LOGIN, 1, 1);
        return obj;
        
    }

}