#include "Util.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void Util::splitStr(vector<string>& res, string& source, string&& fg)
{
    boost::split(res, source, boost::is_any_of(fg));
}

TransObj* Util::getMsgFromInput(string& input)
{
    vector<string> res;
    splitStr(res, input, INPUTSTR_SPLIT_FLAG);
    int n = res.size();
    if (n > 2 || n < 1) { 
        return nullptr;
    }
    TransObj* obj = new TransObj(1, 1, MSG_TALK, 1, 1);
    if (n == 1) { // 此时 是 talk
        // 设置talk对象 recverId_
        return obj;
    }
    string cmdType = res[0];
    string cmd = res[1];
    if (cmdType == "login" || cmdType == "LOGIN") {
        obj->setMsgType(MSG_LOGIN);
        // 设置name password
    } else if (cmdType == "regin" || cmdType == "REGIN") {
        obj->setMsgType(MSG_REG);
        // 设置name password
    } else {
        //  后续
    }
    return obj;

}