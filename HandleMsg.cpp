#include "HandleMsg.h"
#ifdef SERVER_COMPARE
#include "redis_pool.h"
#include "ThreadPool.h"
#include <sys/socket.h>
#include "ComManger.h"
#include "mysql_pool.h"
#include "SeqAbleObj.h"
#include "Player.h"
#endif
#include <string.h>
#include "SeqToBin.h"
#include <vector>
#include <iostream>
using namespace std;

msgHandle g_msgHandle[] = {
    {MSG_CMD, handleMsgCmd},
    {MSG_TALK, handleUserSendMsg},
    {MSG_REG, handleUserRegMsg},
    {MSG_LOGIN, handleUserLogMsg},
    {MSG_ASK_FOR_FRIEND, handleAskForFriendMsg}
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
    cout<<"msg recvId id"<<obj->recverId<<endl;
    cout<<"msg msgType"<<obj->msgType<<endl;
    cout<<"msg len"<<obj->len<<endl;
    cout<<"msg "<<msg<<endl;
#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj] () mutable {
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            SeqToBin::getInstance().getBuff().push(obj);
        }
    });
#endif

#ifdef CLIENT_COMPARE

    cout<<"client  handleUserSendMsg get!"<<msg<<endl;

#endif
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

void handleUserRegMsg(TransObj* obj, int fd)
{
#ifdef SERVER_COMPARE
    cout << hex << (void *)obj << endl;
    if (obj->len > (NAME_MAX_LEN + PASSWORD_MAX_LEN)) {
        return;        
    }
    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        cout << hex << (void *)obj << endl;
        cout<<"do reg!"<<endl;
        cout<<obj->msg<<endl;
        char name[NAME_MAX_LEN] {0};
        char passwd[PASSWORD_MAX_LEN] {0};
        // memcpy(name, obj->msg, 4);
        strncpy(name, obj->msg, NAME_MAX_LEN);
        cout<<"handleUserRegMsg get name is "<<name<<endl;
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password from userinfo where username = \'%s\';", name);
        if (msqlResSet->next()) {
            delete(msqlResSet);
            cout<<"name reg exists already"<<endl;
            // TransObj* tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
            obj->setMsgType(MSG_REG_REFUSE);
            SeqToBin::getInstance().getBuff().push(obj);
            return 0;
        }
        memcpy(passwd, (obj->msg + NAME_MAX_LEN), PASSWORD_MAX_LEN);
        cout<<"handleUserRegMsg get passwd is "<<passwd<<endl;
        bool res = MysqlPool::GetInstance().ExecInsert("insert into userinfo (username, fd, password) values(\'%s\', %d, \'%s\');", name, fd, passwd);
        if (res) {
            auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select max(userid) from userinfo;");
            // TransObj* tansObj = new TransObj(-1, MSG_REG_ACCEPT, 1, fd);
            obj->setMsgType(MSG_REG_ACCEPT);
            if (!msqlResSet->next()) {
                // tansObj->setMsgType(MSG_REG_REFUSE);
                obj->setMsgType(MSG_REG_REFUSE);
            } else {
                int id  = msqlResSet->getInt("userid");
                // tansObj->setId(id);
                obj->setId(id);
                cout<<"handleUserRegMsg insert succed id: "<<id<<endl;
            }
            SeqToBin::getInstance().getBuff().push(obj);
            
            return 1;
        }
        cout<<"handleUserRegMsg insert failed "<<endl;
        // TransObj* tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
        obj->setMsgType(MSG_REG_REFUSE);
        SeqToBin::getInstance().getBuff().push(obj);
        return res ? 1: 0;
    });
#endif

#ifdef CLIENT_COMPARE

    cout<<"client handleUserRegMsg"<<endl;

#endif

}

void handleUserLogMsg(TransObj* obj, int fd)
{
#ifdef SERVER_COMPARE
    cout << hex << (void *)obj << endl;
    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        cout << hex << (void *)obj << endl;
        int id = obj->id;
        cout<<"id "<<id<<endl;
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password, username from userinfo where userid = %d;", id);
        if (msqlResSet == nullptr) {
            cout<<"no find id,"<<id <<endl;
            // TransObj* tansObj = new TransObj(id, MSG_LOGIN_REFUSE, 1, fd);
            // SeqToBin::getInstance().getBuff().push(tansObj);
            obj->setMsgType(MSG_LOGIN_REFUSE);
            SeqToBin::getInstance().getBuff().push(obj);
            delete(msqlResSet);
            return 0;
        }
        if (obj->len > PASSWORD_MAX_LEN) {
            cout<<"password len too long!"<<endl;
            delete(msqlResSet);
            return 0;
        }
        if (msqlResSet->next()) {
            cout<<"now getting the password!"<<endl;
            string relPasswd = msqlResSet->getString("password");
            string userName = msqlResSet->getString("username");
            cout<<"getted the password!"<<endl;
            char tansPasswd[PASSWORD_MAX_LEN];
            memcpy(tansPasswd, (obj->msg + NAME_MAX_LEN), obj->len);
            if (strcmp(tansPasswd, relPasswd.c_str()) != 0) {
                cout<<"password not equal!"<<endl;
                cout<<"realPassword is "<<relPasswd<<endl;
                cout<<"ransPassword is "<<tansPasswd<<endl;
                // TransObj* tansObj = new TransObj(id, MSG_LOGIN_REFUSE, 1, fd);
                // SeqToBin::getInstance().getBuff().push(tansObj);
                obj->setMsgType(MSG_LOGIN_REFUSE);
                SeqToBin::getInstance().getBuff().push(obj);
                delete(msqlResSet);
                return 0;
            }
            ComManger::getInstance().addSessionTalker(obj->id, move(userName), fd);
        }
        cout<<"now do the send thing!"<<endl;
        // TransObj* tansObj = new TransObj(id, MSG_LOGIN_ACCEPT, 1, fd);
        // SeqToBin::getInstance().getBuff().push(tansObj);
        obj->setMsgType(MSG_LOGIN_ACCEPT);
        SeqToBin::getInstance().getBuff().push(obj);
        delete(msqlResSet);
        
        return 1;
    });
#endif

#ifdef CLIENT_COMPARE

    cout<<"client handleUserLogMsg"<<endl;

#endif
}

void handleUserLogOutMsg(TransObj* obj, int fd)
{
#ifdef SERVER_COMPARE
    ThreadPool::getInstance().enqueue([obj] {
        ComManger::getInstance().removeSessionTalker(obj->id);
    });
#endif

#ifdef CLIENT_COMPARE
    cout<<"client handleUserLogOutMsg"<<endl;
#endif
}

void handleAskForFriendMsg(TransObj* obj, int fd) {
#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select userid from userinfo where username = %s;", obj->msg);
        if (msqlResSet->next()) {
            obj->recverId = msqlResSet.getInt("userid");
        } else {
            cout<<"asked friend not found!"<<endl;
            obj->setMsgType(MSG_ASK_FOR_FRIEND_NOT_FOUND);
            obj->setFd(fd);
            SeqToBin::getInstance().getBuff().push(obj);
            return 0;
        }
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            cout<<"servr handleAskForFriendMsg friend online!"<<endl;
            int fd = ComManger::getInstance().getTalkerFd(obj->recverId);
            obj->fd = fd;
            SeqToBin::getInstance().getBuff().push(obj);
            return 1;
        }
        cout<<"servr handleAskForFriendMsg friend offline!"<<endl;
        return 0;
    });

#endif

#ifdef CLIENT_COMPARE

    cout<<"client handleUserLogOutMsg"<<endl;

#endif
}

