#ifndef _UTIL_H_
#define _UTIL_H_
#ifdef CLIENT_COMPARE
#include <iostream>
#include <vector>
#include "SeqAbleObj.h"
#include "Singleton.h"
using namespace std;

#define INPUTSTR_SPLIT_FLAG "@"

typedef shared_ptr<TransObj> (*COM_CMD_HANDLE_FUNC_PTR)(string&& input);
typedef struct CommunicationCmd {

    string comtype;
    COM_CMD_HANDLE_FUNC_PTR handler;

} CommunicationCmdTable;


class Util : public Singleton<Util>
{
public:
    Util(){}

    void splitStr(vector<string>& res, string&& source, string&& fg);
    shared_ptr<TransObj> getMsgFromInput(string&& input);
private:
    
friend class Singleton;
};


shared_ptr<TransObj> handleInputOfLogin(string&& input);
shared_ptr<TransObj> handleInputOfLogout(string&& input);
shared_ptr<TransObj> handleInputOfRegin(string&& input);
shared_ptr<TransObj> handleInputOfAskForFriend(string&& input);
shared_ptr<TransObj> handleInputOfChoseTalker(string&& input);
shared_ptr<TransObj> handleInputOfShowFriends(string&& input);
shared_ptr<TransObj> handleInputOfShowRequests(string&& input);
shared_ptr<TransObj> handleInputOfAccRequest(string&& input);
shared_ptr<TransObj> handleInputOfShowMySelf(string&& input);
bool splitInputStr2NameAndPwd(vector<string>& res, string&& input);
#endif // _UTIL_H_
#endif