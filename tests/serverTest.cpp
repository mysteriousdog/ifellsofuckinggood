#ifdef SERVER_COMPARE

#include "Zepoll.h"
#include "redis_pool.h"
#include <vector>
#include <iostream>
#include "mysql_pool.h"
#include "ThreadPool.h"
#include "HandleMsg.h"
#include "Client.h"
#include "SeqToBin.h"
#include "Player.h"
#include <stdio.h>
using namespace std;


// void showVec(vector<string>& response) {
//     for (size_t loop = 0; loop < response.size(); loop++) {
//         cout<<response[loop]<<" ";
//     }
//     cout<<endl;
// }

int serverInit(size_t threadPoolSize, size_t mysqlPoolSize) {
    cout<<"init   start"<<endl;
    int ret = 0;
    if (!ComManger::getInstance().init()) {
        cout<<"ComManger init err"<<endl;
        return 0;
    }
    ThreadPool::getInstance().init(threadPoolSize);
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", mysqlPoolSize, "user");
    Client* client = new Client();
    auto res = ThreadPool::getInstance().enqueue(*client);

    ServerHandler serverhandler(8877);
	IOLoop::Instance()->start();
    cout<<"init server complete"<<endl;
    return ret;
}

void serverEnd() {
    cout<<"end server  start"<<endl;
    TransObj* obj = new TransObj(1,MSG_BUTTON,3, -1);
    sleep(3);
    SeqToBin::getInstance().getBuff().waitPushTillEmpty(obj);
    cout<<"end server complete"<<endl;
}


void testServerInit(size_t threadPoolSize, size_t mysqlPoolSize) {
    if (!ComManger::getInstance().init()) {
        cout<<"ComManger init err"<<endl;
        return;
    }
    ThreadPool::getInstance().init(threadPoolSize);
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", mysqlPoolSize, "user");
    Client* client = new Client();
    auto res = ThreadPool::getInstance().enqueue(*client);
    cout<<"init server complete"<<endl;
}

void testServerEnd() {
    cout<<"end server  start"<<endl;
    TransObj* obj = new TransObj(1,MSG_BUTTON,3, -1);
    sleep(3);
    SeqToBin::getInstance().getBuff().waitPushTillEmpty(obj);
    cout<<"end server complete"<<endl;
}

bool serverRegTest() {
    char name[] = "zlh";
    char passwd[] = "8219497Pwd!";
    TransObj* obj = new TransObj(1,MSG_REG, sizeof(passwd) + sizeof(name));
    sprintf((obj->msg), name);
    sprintf((obj->msg) + NAME_MAX_LEN, passwd);
    try
    {
        cout << hex << (void *)obj << endl;
        handleUserRegMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        cout<<"err in serverRegTest"<<endl;
        return false;
    }
    return true;
    
}

bool serverLoginTest() {
    char passwd[] = "8219497Pwd!";
    TransObj* obj = new TransObj(2,MSG_LOGIN, sizeof(passwd));
    sprintf((obj->msg) + NAME_MAX_LEN, passwd);
    cout<<"now the input password is "<<obj->msg<<endl;
    try
    {
        cout << hex << (void *)obj << endl;
        handleUserLogMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        cout<<"err in serverLoginTest"<<endl;
        return false;
    }
    
    return true;
}

bool serverLogoutTest() {
    TransObj* obj = new TransObj(1,MSG_LOGOUT, 1);
    try
    {
        cout << hex << (void *)obj << endl;
        handleUserLogOutMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        cout<<"err in serverLogoutTest"<<endl;
        return false;
    }
    
    return true;
}

bool serverAskForFriendTest() {
    TransObj* obj = new TransObj(1,MSG_ASK_FOR_FRIEND, 1);
    obj->setrecverId(2);
    obj->clearMsg();
    obj->setMsg("lily");
    try
    {
        cout << hex << (void *)obj << endl;
        handleAskForFriendMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        cout<<"err in serverLogoutTest"<<endl;
        return false;
    }
    
    return true;
}

bool serverAskForFriendNotFoundTest() {
    TransObj* obj = new TransObj(1,MSG_ASK_FOR_FRIEND, 1);
    snprintf(obj->msg, NAME_MAX_LEN, "xxx");
    try
    {
        handleAskForFriendMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        cout<<"err in serverLogoutTest"<<endl;
        return false;
    }
    return true;
}

bool serverTest() {
    bool ret = true;
    ret = serverRegTest();
    if (!ret) {
        return ret;
    }
    ret = serverLoginTest();
    if (!ret) {
        return ret;
    }
    return true;
}

void mysqlTest() {
    bool res = MysqlPool::GetInstance().ExecInsert("insert into userinfo (username, fd, password) values(\'%s\', %d, \'%s\');", "name", 10, "123");
    assert(res == true);
    auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select max(userid) as id from userinfo;");
    res = msqlResSet->next();
    assert(res == true);
    int id  = msqlResSet->getInt("id");
    assert(id > 0);
    cout<<"id "<<id;
    msqlResSet = MysqlPool::GetInstance().ExecQuery("select password from userinfo where username = \'%s\';", "name");
    res = msqlResSet->next();
    assert(res == true);
    auto passwd = msqlResSet->getString("password");
    assert(passwd == "123");
}

int main(int argc, char** argv) {
    // testServerInit(4, 4);
    // mysqlTest();
    // testServerEnd();

    serverInit(4, 4);
    serverEnd();
	return 0;
}
#endif