#include "HandleMsg.h"
#ifdef SERVER_COMPARE
#include "redis_pool.h"
#include <sys/socket.h>
#include "ComManger.h"
#endif
#include "SeqToBin.h"
#include <vector>
using namespace std;

msgHandle g_msgHandle[] = {

    {MSG_CMD, handleMsgCmd},
    {MSG_USER_RECV, handleUserSendMsg}
};

void MsgHandler::handle(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        return;
    }
    for (int loop = 0; loop < sizeof(g_msgHandle) / sizeof(msgHandle); loop++) {
        cout<<"now the g_msgHandle msgType is "<<g_msgHandle[loop].msgTye <<endl;
        cout<<"now the obj msgType is "<<obj->msgType <<endl;
        if (g_msgHandle[loop].msgTye == obj->msgType) {
            g_msgHandle[loop].handler(obj, fd);
        }
    }
}

void handleUserSendMsg(TransObj* obj, int fd) {
    cout<<"get in handleUserSendMsg "<<endl;
    char *msg = obj->msg;
    int id = obj->id;
    cout<<"msg id"<<obj->id<<endl;
    cout<<"msg msgType"<<obj->msgType<<endl;
    cout<<"msg len"<<obj->len<<endl;
    cout<<"msg "<<msg<<endl;
#ifdef SERVER_COMPARE
    vector<string> response;
    if (KGRedisClient::getInstance().ExecHset(response, "userHash", to_string(id), to_string(fd))) {
        cout<<"Set redis success response is: "<<endl;
    } else {
        cout<<"Set redis err response is: "<<endl;
    }
#endif
    // auto userMap = ComManger::getInstance().getAllUserMap();
    // // SeqToBin& seq = SeqToBin::getInstance();
    // for (auto iter = userMap.begin(); iter != userMap.end(); iter++) {
    //     // if (send(iter->second, msg, obj->len, 0) != obj->len)
    //     // {
    //     //     std::cout << "Error writing to socket" << std::endl;
    //     // }
    //     std::cout << "end "<<iter->second<<msg<<std::endl;
    // }
    // string response;
    // if (KGRedisClient::getInstance().ExecSetString(response, "userid", to_string(fd))) {
    //     cout<<"Set redis success response is: "<<response<<endl;
    // } else {
    //     cout<<"Set redis err response is: "<<response<<endl;
    // }
    // if (KGRedisClient::getInstance().ExecGetString(response, "userid")) {
    //     cout<<"Get redis success response is: "<<response<<endl;
    // } else {
    //     cout<<"Get redis err response is: "<<response<<endl;
    // }
}

void handleMsgCmd(TransObj* obj, int fd) {
    cout<<"get in handleMsgCmd "<<endl;
    Command *cmd = (Command*)obj->msg;
    cout<<"get in handleMsgCmd2 "<<endl;
    cout<<"handle msg cmd "<<(int)cmd->getType()<<endl;
#ifdef CLIENT_COMPARE
    if (fd == -1) {
        // 此时是客户端
        cout<<"此时是客户端"<<endl;
        return;
    }
#endif
#ifdef SERVER_COMPARE
    auto userMap = ComManger::getInstance().getAllUserMap();
    // SeqToBin& seq = SeqToBin::getInstance();
    for (auto iter = userMap.begin(); iter != userMap.end(); iter++) {
        if (send(iter->second, (void*)obj, obj->len, 0) != obj->len)
        {
            std::cout << "Error writing to socket" << std::endl;
        }
        std::cout << "end "<<iter->second<<" "<<cmd->getType()<<std::endl;
    }
#endif
}