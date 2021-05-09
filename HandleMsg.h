#ifndef _HANDLE_MSG_H_
#define _HANDLE_MSG_H_

#include "MyEnum.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include "Singleton.h"
#include <iostream>
using namespace std;

typedef void (*MSG_HANDLE_FUNC_PTR)(TransObj*, int);

typedef struct MsgHandle{

    MSG_TYPE_UINT32_ENUM msgTye;
    MSG_HANDLE_FUNC_PTR handler;

} msgHandle;

void handleMsgCmd(TransObj* obj, int fd);
void handleUserSendMsg(TransObj* obj, int fd);



class MsgHandler : public Singleton<MsgHandler>
{
public:

    void handle(TransObj* obj, int fd);

private:
    MsgHandler(){};

friend class Singleton;
};





#endif