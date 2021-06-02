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
typedef std::function<void(TransObj*, int)> MSG_HANDLE_FUNC_PTR;

typedef struct MsgHandle{

    MSG_TYPE_UINT32_ENUM msgTye;
    MSG_HANDLE_FUNC_PTR handler;

} msgHandle;

void handleMsgCmd(TransObj* obj, int fd);
void handleUserConnectMsg(TransObj* obj, int fd);
void handleUserSendMsg(TransObj* obj, int fd);
void handleUserRegMsg(TransObj* obj, int fd);
void handleUserLogMsg(TransObj* obj, int fd);
void handleUserLogRefusedMsg(TransObj* obj, int fd);
void handleUserLogAcceptedMsg(TransObj* obj, int fd);
void handleUserLogOutMsg(TransObj* obj, int fd);
void handleAskForFriendMsg(TransObj* obj, int fd);
void handleAskForFriendNotFoundMsg(TransObj* obj, int fd);
void handleAskForFriendAcceptMsg(TransObj* obj, int fd);
void handleUserRegRefusedMsg(TransObj* obj, int fd);
void handleUserRegAcceptedMsg(TransObj* obj, int fd);

class MsgHandler : public Singleton<MsgHandler>
{
public:

    void handle(TransObj* obj, int fd);

private:
    MsgHandler(){};

friend class Singleton;
};





#endif