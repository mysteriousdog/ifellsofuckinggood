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

#ifdef CLIENT_COMPARE
#include "Util.h"
#include "SysManger.h"
#include "IOManger.h"
#include <sstream>
#include "Player.h"
#include <sstream>
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
    {MSG_REG_REFUSE, handleUserRegRefusedMsg},
    {MSG_REG_ACCEPT, handleUserRegAcceptedMsg},
    {MSG_LOGIN, handleUserLogMsg},
    {MSG_LOGIN_REFUSE, handleUserLogRefusedMsg},
    {MSG_LOGIN_ACCEPT, handleUserLogAcceptedMsg},
    {MSG_ASK_FOR_FRIEND, handleAskForFriendMsg},
    {MSG_ASK_FOR_FRIEND_NOT_FOUND, handleAskForFriendNotFoundMsg},
    {MSG_ASK_FOR_FRIEND_ACCEPT, handleAskForFriendAcceptMsg}
};

void MsgHandler::handle(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        return;
    }
    for (int loop = 0; loop < sizeof(g_msgHandle) / sizeof(msgHandle); loop++) {
        if (g_msgHandle[loop].msgTye == obj->msgType) {
            cout<<"now the obj msgType is "<<obj->msgType <<endl;
            g_msgHandle[loop].handler(obj, fd);
            break;
        }
    }
}

void handleUserConnectMsg(TransObj* obj, int fd) {
    if (obj == nullptr) {
        cout<<"get in handleUserConnectMsg err nullptr"<<endl;
        return;
    }
    cout<<"get in handleUserConnectMsg "<<endl;
#ifdef SERVER_COMPARE



#endif

}

void handleUserSendMsg(TransObj* obj, int fd) {

    if (obj == nullptr) {
        cout<<"get in handleUserSendMsg err nullptr"<<endl;
        return;
    }
    cout<<"get in handleUserSendMsg "<<endl;
#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj] () mutable {
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            SeqToBin::getInstance().getBuff().push(obj);
        }
    });
#endif

#ifdef CLIENT_COMPARE

    cout<<"client  handleUserSendMsg get!"<<endl;
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"You just got message from ";
    (*ss)<<obj->id<<" :\n";
    (*ss)<<obj->msg<<" \n";
    cout<<"client  handleUserSendMsg get "<<ss->str().c_str()<<endl;
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    delete(obj);

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
        cout<<"此时是客户端 handleMsgCmd"<<endl;
        
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
    if (obj == nullptr) {
        cout<<"client handleUserRegMsg err nullptr!"<<endl;
        return;
    }
#ifdef SERVER_COMPARE
    if (obj->len > (NAME_MAX_LEN + PASSWORD_MAX_LEN)) {
        return;        
    }
    char str[MAX_TRANS_MSG_LEN] {0};
    snprintf(str, MAX_TRANS_MSG_LEN, obj->getMsg());
    cout<<"str in handleUserRegMsg "<<str<<endl;
    ThreadPool::getInstance().enqueue([str, fd] () mutable {
        cout<<"do reg!  fd-> "<<int(fd)<<endl;
        TransObj* sendObj = new TransObj();
        sendObj->setFd(fd);
        char name[NAME_MAX_LEN] {0};
        char passwd[PASSWORD_MAX_LEN] {0};
        strncpy(name, str, NAME_MAX_LEN);
        cout<<"handleUserRegMsg get name is "<<name<<endl;
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password from userinfo where username = \'%s\';", name);
        if (msqlResSet->next()) {
            delete(msqlResSet);
            cout<<"name reg exists already"<<endl;
            // TransObj* tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
            sendObj->setMsgType(MSG_REG_REFUSE);
            sendObj->setMsg("regin err! name reg exists already!");
            SeqToBin::getInstance().getBuff().push(sendObj);
            return 0;
        }
        memcpy(passwd, (str + NAME_MAX_LEN), PASSWORD_MAX_LEN);
        cout<<"handleUserRegMsg get passwd is "<<passwd<<endl;
        bool res = MysqlPool::GetInstance().ExecInsert("insert into userinfo (username, fd, password) values(\'%s\', %d, \'%s\');", name, int(fd), passwd);
        if (res) {
            auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select max(userid) as id from userinfo;");
            // TransObj* tansObj = new TransObj(-1, MSG_REG_ACCEPT, 1, fd);
            sendObj->setMsgType(MSG_REG_ACCEPT);
            if (!msqlResSet->next()) {
                // tansObj->setMsgType(MSG_REG_REFUSE);
                sendObj->setMsgType(MSG_REG_REFUSE);
                sendObj->setMsg("regin err no found current id! \n");
            } else {
                int id  = msqlResSet->getInt("id");
                // tansObj->setId(id);
                sendObj->setId(id);
                sendObj->setMsg("regin accepted! \n");
                cout<<"handleUserRegMsg insert succed id: "<<id<<endl;
            }
            SeqToBin::getInstance().getBuff().push(sendObj);
            
            return 1;
        }
        cout<<"handleUserRegMsg insert failed "<<endl;
        // TransObj* tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
        sendObj->setMsgType(MSG_REG_REFUSE);
        sendObj->setMsg("regin err! insert error!\n");
        SeqToBin::getInstance().getBuff().push(sendObj);
        return res ? 1: 0;
    });
#endif

#ifdef CLIENT_COMPARE
    
    cout<<"CLIENT_COMPARE handleUserRegMsg"<<endl;

#endif

}

void handleUserRegRefusedMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"client handleUserRegRefusedMsg err nullptr!"<<endl;
        return;
    }
#ifdef CLIENT_COMPARE 

    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<obj->getMsg();
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
}

void handleUserRegAcceptedMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"client handleUserRegAcceptedMsg err nullptr!"<<endl;
        return;
    }

#ifdef CLIENT_COMPARE 

    // Player::getInstance().setLoginStatus(false);
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    int id = obj->getId();
    Player::getInstance().setPlayerId(id);
    (*ss)<<"Regin success!\n";
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
 
}


void handleUserLogMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"get in handleUserLogMsg err nullptr"<<endl;
        return;
    }
#ifdef SERVER_COMPARE
    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        int id = obj->id;
        cout<<"id "<<id<<endl;
        TransObj* sendObj = new TransObj();
        sendObj->setId(id);

        if (obj->len > MAX_TRANS_MSG_LEN) {
            cout<<"msg len too long!"<<endl;
            sendObj->setMsgType(MSG_LOGIN_REFUSE);
            sendObj->setMsg("Login err! msg len too long! \n");
            SeqToBin::getInstance().getBuff().push(sendObj);
            return 0;
        }
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password, username from userinfo where userid = %d;", id);
        if (msqlResSet == nullptr) {
            cout<<"no find id,"<<id <<endl;
            // TransObj* tansObj = new TransObj(id, MSG_LOGIN_REFUSE, 1, fd);
            // SeqToBin::getInstance().getBuff().push(tansObj);
            sendObj->setMsgType(MSG_LOGIN_REFUSE);
            sendObj->setMsg("Login err! not found id\n");
            SeqToBin::getInstance().getBuff().push(sendObj);
            delete(msqlResSet);
            return 0;
        }
        if (msqlResSet->next()) {
            cout<<"now getting the password!"<<endl;
            string relPasswd = msqlResSet->getString("password");
            string userName = msqlResSet->getString("username");
            cout<<"getted the password!"<<endl;
            char* tansPasswd = obj->getPasswd();

            if (strcmp(tansPasswd, relPasswd.c_str()) != 0) {
                cout<<"password not equal!"<<endl;
                cout<<"realPassword is "<<relPasswd<<endl;
                cout<<"ransPassword is "<<tansPasswd<<endl;
                // TransObj* tansObj = new TransObj(id, MSG_LOGIN_REFUSE, 1, fd);
                // SeqToBin::getInstance().getBuff().push(tansObj);
                sendObj->setMsgType(MSG_LOGIN_REFUSE);
                sendObj->setMsg("Login err! password not equal!\n");
                SeqToBin::getInstance().getBuff().push(sendObj);
                delete(msqlResSet);
                return 0;
            }
            ComManger::getInstance().addSessionTalker(sendObj->id, move(userName), fd);
        }
        cout<<"now do the send thing!"<<endl;
        // TransObj* tansObj = new TransObj(id, MSG_LOGIN_ACCEPT, 1, fd);
        // SeqToBin::getInstance().getBuff().push(tansObj);
        sendObj->setMsgType(MSG_LOGIN_ACCEPT);
        SeqToBin::getInstance().getBuff().push(sendObj);
        delete(msqlResSet);
        
        return 1;
    });
#endif

#ifdef CLIENT_COMPARE

    cout<<" client handleUserLogMsg"<<endl;
#endif
}

void handleUserLogRefusedMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"get in handleUserLogRefusedMsg err nullptr"<<endl;
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<obj->msg;
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    delete(obj);

#endif
}

void handleUserLogAcceptedMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"get in handleUserLogAcceptedMsg err nullptr"<<endl;
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"you have login in success!\n";
    Player::getInstance().setLoginStatus(true);
    Player::getInstance().setPlayerId(obj->id);
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    delete(obj);
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

    if (obj == nullptr) {
        cout<<"get in handleAskForFriendMsg err nullptr"<<endl;
        return;
    }

#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select userid from userinfo where username = \'%s\';", obj->msg);
        if (msqlResSet->next()) {
            obj->recverId = msqlResSet->getInt("userid");
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

    cout<<"client handleAskForFriendMsg"<<endl;
    if (obj == nullptr) {
        return;
    }
    char* name = obj->msg;
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"You have a request from ";
    (*ss)<<name;
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    SysManger::getInstance().pushBackReq(obj);
#endif
}

void handleAskForFriendNotFoundMsg(TransObj* obj, int fd)
{
    if (obj == nullptr) {
        cout<<"get in handleAskForFriendNotFoundMsg err nullptr"<<endl;
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"the friend you asked " <<obj->msg<< " do not exists!\n";
    SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    delete(obj);
    
#endif
}


void handleAskForFriendAcceptMsg(TransObj* obj, int fd)
{

    if (obj == nullptr) {
        cout<<"get in handleAskForFriendAcceptMsg err nullptr"<<endl;
        return;
    }
#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj] () mutable {
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            int fd = ComManger::getInstance().getTalkerFd(obj->recverId);
            obj->setFd(fd);
            string name;
            if (ComManger::getInstance().getTalkerName(obj->id, name)) {
                if (name.length() >= NAME_MAX_LEN) {
                    cout<<"server handleAskForFriendAcceptMsg get name length err "<<name.length()<<endl;
                    return -1;
                }
                obj->setMsg(name.c_str());
            }
            SeqToBin::getInstance().getBuff().push(obj);
            return 1;
        }
        cout<<"server handleAskForFriendAcceptMsg get recverId err "<<endl;
        return -1;
    });

#endif

#ifdef CLIENT_COMPARE

    string name = obj->getName();
    if (name.length() >= NAME_MAX_LEN) {
        cout<<"client handleAskForFriendAcceptMsg get name length err "<<name.length()<<endl;
        return;
    }
    if (Player::getInstance().addFriend(move(name), obj->id)) {
        shared_ptr<stringstream> ss = make_shared<stringstream>();
        (*ss)<<"Your ask friend request had been accepted from "<<name<<"\n";
        SystemMsgObj *sysObj = new SystemMsgObj(SYS_OUTPUT_MSG, ss);
        SeqToBin::getInstance().putSysMsg(sysObj);
    } else {
        cout<<"client handleAskForFriendAcceptMsg add  friend err "<<name<<endl;
    }
#endif
}