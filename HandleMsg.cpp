#include "HandleMsg.h"

msgHandle g_msgHandle[] = {

    {MSG_CMD, handleMsgCmd},
    {MSG_USER_SEND, handleUserSendMsg}
};

void MsgHandler::handle(TransObj* obj)
{
    if (obj == nullptr) {
        return;
    }
    for (int loop = 0; loop < sizeof(g_msgHandle) / sizeof(msgHandle); loop++) {
        cout<<"now the g_msgHandle msgType is "<<g_msgHandle[loop].msgTye <<endl;
        cout<<"now the obj msgType is "<<obj->msgType <<endl;
        if (g_msgHandle[loop].msgTye == obj->msgType) {
            g_msgHandle[loop].handler(obj);
        }
    }
}

void handleUserSendMsg(TransObj* obj) {
    cout<<"get in handleUserSendMsg "<<endl;
    char *msg = obj->msg;
    cout<<"msg id"<<obj->id<<endl;
    cout<<"msg msgType"<<obj->msgType<<endl;
    cout<<"msg len"<<obj->len<<endl;
    cout<<"msg "<<msg<<endl;   
}

void handleMsgCmd(TransObj* obj) {
    cout<<"get in handleMsgCmd "<<endl;
    Command *cmd = (Command*)obj->msg;
    cout<<"get in handleMsgCmd2 "<<endl;
    cout<<"handle msg cmd "<<(int)cmd->getType()<<endl;
}