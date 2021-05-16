#define TEST 1
#if not TEST
#include "Zepoll.h"
#include "mysql_pool.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    if (!ComManger::getInstance().init()) {
        cout<<"ComManger init err"<<endl;
        return 1;
    }
    MysqlPool::GetInstance().initPool("127.0.0.1", "root", "353656535132Zlh!", 4);
	ServerHandler serverhandler(8877);
	IOLoop::Instance()->start();
	return 0;
}
#endif

#if TEST
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
    ThreadPool::getInstance().init(threadPoolSize);
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", mysqlPoolSize, "user");
    Client client;
    auto res = ThreadPool::getInstance().enqueue(client);
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

bool test() {
    char passwd[] = "8219497Zwd!";
    TransObj* obj = new TransObj(1,MSG_LOGIN, sizeof(passwd));
    sprintf((obj->msg) + NAME_MAX_LEN, passwd);
    cout<<"now the input password is "<<obj->msg<<endl;
    try
    {
        handleUserLogMsg(obj, -1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    return true;
}

int main(int argc, char** argv) {
    serverInit(2, 2);
    char name[] = "zlh2";
    char passwd[] = "8219497Pwd!";
    TransObj* obj = new TransObj(1,MSG_REG, sizeof(passwd) + sizeof(name));
    sprintf((obj->msg), name);
    sprintf((obj->msg) + NAME_MAX_LEN, passwd);
    cout<<"!!!!!!!!"<<obj->msg<<endl;
    handleUserRegMsg(obj, -1);
    serverEnd();
    // MysqlPool* mysqlPool = new MysqlPool(); ThreadPool::getInstanch()
    // mysqlPool->initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", 2);
    // ThreadPool threadPool(2);
    // ThreadPool::getInstance().enqueue([] {
    //     // auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select * from userinfo;");
    //     // if (msqlResSet == nullptr) {
    //     //     return;
    //     // }
    //     // int i = 0;
    //     // cout<<"msqlResSet->next()"<<endl;
    //     // cout<<msqlResSet->next()<<endl;
    //     // while (msqlResSet->next()) {
    //     //     cout<<i<<endl;
    //     //     cout<<"userid "<<msqlResSet->getInt("userid");
    //     //     cout<<"username "<<msqlResSet->getString("username");
    //     //     cout<<"fd "<<msqlResSet->getInt("fd");
    //     //     cout<<"pwd "<<msqlResSet->getString("password");
    //     // }
    //     cout<<"thread"<<endl;
    // });

    // auto msqlResSet = MysqlPool::GetInstance().ExecQuery("select * from userinfo;");
    //     if (msqlResSet == nullptr) {
    //         cout<<"msqlResSet->next()11"<<endl;
    //         return 0;
    //     }
    //     int i = 0;
    //     cout<<"msqlResSet->next()"<<endl;
    //     cout<<msqlResSet->next()<<endl;
    //     while (msqlResSet->next()) {
    //         cout<<i<<endl;
    //         cout<<"userid "<<msqlResSet->getInt("userid");
    //         cout<<"username "<<msqlResSet->getString("username");
    //         cout<<"fd "<<msqlResSet->getInt("fd");
    //         cout<<"pwd "<<msqlResSet->getString("password");
    //     }
	return 0;
}
#endif