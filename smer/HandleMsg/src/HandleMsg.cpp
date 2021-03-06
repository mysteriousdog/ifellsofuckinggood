#include "HandleMsg.h"
#include "Log.h"
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

void MsgHandler::handle(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        return;
    }
    for (int loop = 0; loop < sizeof(g_msgHandle) / sizeof(msgHandle); loop++) {
        if (g_msgHandle[loop].msgTye == obj->msgType) {
            LOG_DEBUG("now the obj msgType is " + to_string(obj->msgType ));
            g_msgHandle[loop].handler(obj, fd);
            break;
        }
    }
}

void handleUserConnectMsg(shared_ptr<TransObj>obj, int fd) {
    if (obj == nullptr) {
        return;
    }
#ifdef SERVER_COMPARE



#endif

}

void handleUserSendMsg(shared_ptr<TransObj>obj, int fd) {

    if (obj == nullptr) {
        LOG_ERR("get in handleUserSendMsg err nullptr ");
        return;
    }
#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj] () mutable {
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            SeqToBin::getInstance().getBuff().push(obj);
        }
    });
#endif

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"You just got message from ";
    (*ss)<<obj->id<<" :\n";
    (*ss)<<obj->msg<<" \n";
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
}

void handleMsgCmd(shared_ptr<TransObj>obj, int fd) {
    Command *cmd = (Command*)obj->msg;
#ifdef CLIENT_COMPARE
    if (fd == -1) {
        // ??????????????????
        
        return;
    }
#endif
#ifdef SERVER_COMPARE
    // auto userMap = ComManger::getInstance().getAllUserMap();
    // // SeqToBin& seq = SeqToBin::getInstance();
    // for (auto iter = userMap.begin(); iter != userMap.end(); iter++) {
    //     if (send(iter->second, (void*)obj, obj->len, 0) != obj->len)
    //     {
    //         LOG_ERR("Error writing to socket" );
    //     }
    // }
#endif
}

void handleUserRegMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("client handleUserRegMsg err nullptr!");
        return;
    }
#ifdef SERVER_COMPARE
    if (obj->len > (NAME_MAX_LEN + PASSWORD_MAX_LEN)) {
        return;        
    }

    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        LOG_INFO("do reg!  fd-> " + to_string(fd));
        auto sendObj = make_shared<TransObj>();
        obj->setFd(fd);
        const char* name = obj->getName();
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password from userinfo where username = \'%s\';", name);
        if (msqlResSet->next()) {
            delete(msqlResSet);
            LOG_ERR("name reg exists already " + string(name));
            // shared_ptr<TransObj>tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
            obj->setMsgType(MSG_REG_REFUSE);
            obj->setMsg("regin err! name reg exists already!");
            SeqToBin::getInstance().getBuff().push(obj);
            return 0;
        }
        const char* passwd = obj->getPasswd();
        LOG_INFO("handleUserRegMsg get passwd is-> " + string(passwd));
        bool res = MysqlPool::GetInstance().ExecInsert("insert into userinfo (username, fd, password) values(\'%s\', %d, \'%s\');", name, int(fd), passwd);
        if (res) {
            auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select max(userid) as id from userinfo;");
            // shared_ptr<TransObj>tansObj = new TransObj(-1, MSG_REG_ACCEPT, 1, fd);
            obj->setMsgType(MSG_REG_ACCEPT);
            if (!msqlResSet->next()) {
                // tansObj->setMsgType(MSG_REG_REFUSE);
                obj->setMsgType(MSG_REG_REFUSE);
                obj->setMsg("regin err no found current id! \n");
            } else {
                int id  = msqlResSet->getInt("id");
                // tansObj->setId(id);
                obj->setId(id);
                obj->setMsg("regin accepted! \n");
            }
            SeqToBin::getInstance().getBuff().push(obj);
            
            return 1;
        }
        LOG_ERR("handleUserRegMsg insert failed ");
        // shared_ptr<TransObj>tansObj = new TransObj(-1, MSG_REG_REFUSE, 1, fd);
        obj->setMsgType(MSG_REG_REFUSE);
        obj->setMsg("regin err! insert error!\n");
        SeqToBin::getInstance().getBuff().push(obj);
        return res ? 1: 0;
    });
#endif

#ifdef CLIENT_COMPARE
    

#endif

}

void handleUserRegRefusedMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("client handleUserRegRefusedMsg err nullptr!");
        return;
    }
#ifdef CLIENT_COMPARE 

    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<obj->getMsg();
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
}

void handleUserRegAcceptedMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("client handleUserRegAcceptedMsg err nullptr!");
        return;
    }

#ifdef CLIENT_COMPARE 

    shared_ptr<stringstream> ss = make_shared<stringstream>();
    int id = obj->getId();
    Player::getInstance().setPlayerId(id);
    (*ss)<<"Regin success!\n";
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
 
}


void handleUserLogMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("get in handleUserLogMsg err nullptr");
        return;
    }
#ifdef SERVER_COMPARE
    ThreadPool::getInstance().enqueue([obj, fd] () mutable {

        if (obj->len > MAX_TRANS_MSG_LEN) {
            LOG_ERR("msg len too long! -> " + to_string(obj->len));
            obj->setMsgType(MSG_LOGIN_REFUSE);
            obj->setMsg("Login err! msg len too long! \n");
            SeqToBin::getInstance().getBuff().push(obj);
            return 0;
        }
        const char* name = obj->getName();
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select password, userid from userinfo where username = \'%s\';", name);
        if (msqlResSet == nullptr) {
            LOG_ERR("no find name -> " + string(name));
            // shared_ptr<TransObj>tansObj = new TransObj(id, MSG_LOGIN_REFUSE, 1, fd);
            // SeqToBin::getInstance().getBuff().push(tansObj);
            obj->setMsgType(MSG_LOGIN_REFUSE);
            obj->setMsg("Login err! not found name\n");
            SeqToBin::getInstance().getBuff().push(obj);
            delete(msqlResSet);
            return 0;
        }
        if (msqlResSet->next()) {
            string relPasswd = msqlResSet->getString("password");
            const char* tansPasswd = obj->getPasswd();

            if (strcmp(tansPasswd, relPasswd.c_str()) != 0) {
                LOG_INFO("password not equal! -> " + string(tansPasswd));
                obj->setMsgType(MSG_LOGIN_REFUSE);
                obj->setMsg("Login err! password not equal!\n");
                SeqToBin::getInstance().getBuff().push(obj);
                delete(msqlResSet);
                return 0;
            }
            int id = -1;
            try
            {
                id = msqlResSet->getInt("userid");
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                LOG_INFO("id not find! -> " + string(e.what()));
                obj->setMsgType(MSG_LOGIN_REFUSE);
                obj->setMsg("Login err! id not found!\n");
                SeqToBin::getInstance().getBuff().push(obj);
                delete(msqlResSet);
                return 0;
            }
            obj->setId(id);
            obj->setFd(fd);
            ComManger::getInstance().addSessionTalker(obj->id, string(name), fd);
        }
        obj->setMsgType(MSG_LOGIN_ACCEPT);
        SeqToBin::getInstance().getBuff().push(obj);
        delete(msqlResSet);
        
        return 1;
    });
#endif

#ifdef CLIENT_COMPARE

#endif
}

void handleUserLogRefusedMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("get in handleUserLogRefusedMsg err nullptr");
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<obj->msg;
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);

#endif
}

void handleUserLogAcceptedMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("get in handleUserLogAcceptedMsg err nullptr");
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"you have login in success!\n";
    LOG_INFO("login now " + to_string(obj->getId()) + " " + obj->getName() + " " + obj->getPasswd());
    Player::getInstance().setPlayerLogin(obj->getId(), obj->getName(), obj->getPasswd());
    Player::getInstance().savePlayerData();
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
#endif
}


void handleUserLogOutMsg(shared_ptr<TransObj>obj, int fd)
{
#ifdef SERVER_COMPARE
    ThreadPool::getInstance().enqueue([obj] {
        ComManger::getInstance().removeSessionTalker(obj->id);
    });
#endif

#ifdef CLIENT_COMPARE

#endif
}

void handleAskForFriendMsg(shared_ptr<TransObj>obj, int fd) {

    if (obj == nullptr) {
        LOG_ERR("get in handleAskForFriendMsg err nullptr");
        return;
    }

#ifdef SERVER_COMPARE

    ThreadPool::getInstance().enqueue([obj, fd] () mutable {
        auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select userid from userinfo where username = \'%s\';", obj->msg);
        if (msqlResSet->next()) {
            obj->recverId = msqlResSet->getInt("userid");
        } else {
            LOG_INFO("asked friend not found!");
            obj->setMsgType(MSG_ASK_FOR_FRIEND_NOT_FOUND);
            obj->setFd(fd);
            SeqToBin::getInstance().getBuff().push(obj);
            return 0;
        }
        if (ComManger::getInstance().isTalkerOnline(obj->recverId)) {
            int fd = ComManger::getInstance().getTalkerFd(obj->recverId);
            obj->fd = fd;

            SeqToBin::getInstance().getBuff().push(obj);
            return 1;
        }
        LOG_INFO("servr handleAskForFriendMsg friend offline!");
        return 0;
    });

#endif

#ifdef CLIENT_COMPARE

    if (obj == nullptr) {
        return;
    }
    char* name = obj->msg;
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"You have a request from ";
    (*ss)<<name;
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    SysManger::getInstance().pushBackReq(obj);
#endif
}

void handleAskForFriendNotFoundMsg(shared_ptr<TransObj>obj, int fd)
{
    if (obj == nullptr) {
        LOG_ERR("get in handleAskForFriendNotFoundMsg err nullptr");
        return;
    }

#ifdef CLIENT_COMPARE
    shared_ptr<stringstream> ss = make_shared<stringstream>();
    (*ss)<<"the friend you asked " <<obj->msg<< " do not exists!\n";
    auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
    SeqToBin::getInstance().putSysMsg(sysObj);
    
#endif
}


void handleAskForFriendAcceptMsg(shared_ptr<TransObj>obj, int fd)
{

    if (obj == nullptr) {
        LOG_ERR("get in handleAskForFriendAcceptMsg err nullptr");
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
                    LOG_INFO("server handleAskForFriendAcceptMsg get name length err " + name);
                    return -1;
                }
                obj->setMsg(name.c_str());
            }
            SeqToBin::getInstance().getBuff().push(obj);
            return 1;
        }
        LOG_ERR("server handleAskForFriendAcceptMsg get recverId err ");
        return -1;
    });

#endif

#ifdef CLIENT_COMPARE

    string name = obj->getName();
    if (name.length() >= NAME_MAX_LEN) {
        LOG_ERR("client handleAskForFriendAcceptMsg get name length err " + name);
        return;
    }
    if (Player::getInstance().addFriend(move(name), obj->id)) {
        shared_ptr<stringstream> ss = make_shared<stringstream>();
        (*ss)<<"Your ask friend request had been accepted from "<<name<<"\n";
        auto sysObj = make_shared<SystemMsgObj>(SYS_OUTPUT_MSG, ss);
        SeqToBin::getInstance().putSysMsg(sysObj);
    } else {
        LOG_INFO("client handleAskForFriendAcceptMsg add  friend err " + name);
    }
#endif
}