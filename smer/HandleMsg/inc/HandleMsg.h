#ifndef _HANDLE_MSG_H_
#define _HANDLE_MSG_H_

#include "MyEnum.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include "Singleton.h"
#include <iostream>
#include <functional>
using namespace std;

// typedef void (*MSG_HANDLE_FUNC_PTR)(TransObj*, int);
typedef std::function<void(shared_ptr<TransObj>, int)> MSG_HANDLE_FUNC_PTR;

typedef struct MsgHandle{

    MSG_TYPE_UINT32_ENUM msgTye;
    MSG_HANDLE_FUNC_PTR handler;

} msgHandle;

void handleMsgCmd(shared_ptr<TransObj> obj, int fd);
void handleUserConnectMsg(shared_ptr<TransObj> obj, int fd);
void handleUserSendMsg(shared_ptr<TransObj>obj, int fd);
void handleUserRegMsg(shared_ptr<TransObj>obj, int fd);
void handleUserLogMsg(shared_ptr<TransObj>obj, int fd);
void handleUserLogRefusedMsg(shared_ptr<TransObj>obj, int fd);
void handleUserLogAcceptedMsg(shared_ptr<TransObj>obj, int fd);
void handleUserLogOutMsg(shared_ptr<TransObj>obj, int fd);
void handleAskForFriendMsg(shared_ptr<TransObj>obj, int fd);
void handleAskForFriendNotFoundMsg(shared_ptr<TransObj>obj, int fd);
void handleAskForFriendAcceptMsg(shared_ptr<TransObj>obj, int fd);
void handleUserRegRefusedMsg(shared_ptr<TransObj>obj, int fd);
void handleUserRegAcceptedMsg(shared_ptr<TransObj>obj, int fd);

class MsgHandler : public Singleton<MsgHandler>
{
public:

    void handle(shared_ptr<TransObj>obj, int fd);

private:
    MsgHandler(){};

friend class Singleton;
};





#endif