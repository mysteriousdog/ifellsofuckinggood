#ifndef _UTIL_H_
#define _UTIL_H_
#ifdef CLIENT_COMPARE
#include <iostream>
#include <vector>
#include "SeqAbleObj.h"
#include "Singleton.h"
using namespace std;

#define INPUTSTR_SPLIT_FLAG "@"

typedef TransObj* (*COM_CMD_HANDLE_FUNC_PTR)(string&& input);
typedef struct CommunicationCmd {

    string comtype;
    COM_CMD_HANDLE_FUNC_PTR handler;

} CommunicationCmdTable;


class Util : public Singleton<Util>
{
public:
    Util(){}

    void splitStr(vector<string>& res, string&& source, string&& fg);
    TransObj* getMsgFromInput(string&& input);
private:
    
friend class Singleton;
};


TransObj* handleInputOfLogin(string&& input);
TransObj* handleInputOfLogout(string&& input);
TransObj* handleInputOfRegin(string&& input);
TransObj* handleInputOfAskForFriend(string&& input);
TransObj* handleInputOfChoseTalker(string&& input);
TransObj* handleInputOfShowFriends(string&& input);
TransObj* handleInputOfShowRequests(string&& input);
TransObj* handleInputOfAccRequest(string&& input);
TransObj* handleInputOfShowMySelf(string&& input);
bool splitInputStr2NameAndPwd(vector<string>& res, string&& input);
#endif // _UTIL_H_
#endif