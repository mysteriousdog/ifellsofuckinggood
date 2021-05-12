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
using namespace std;


// void showVec(vector<string>& response) {
//     for (size_t loop = 0; loop < response.size(); loop++) {
//         cout<<response[loop]<<" ";
//     }
//     cout<<endl;
// }

int serverInit(size_t threadPoolSize) {
    int ret = 0;
    ThreadPool::getInstance().init(threadPoolSize);
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", 2, "user");
    return ret;
}

int main(int argc, char** argv) {
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", 2, "user");
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
    cout<<"server1"<<endl;
    ThreadPool::getInstance().init(2);
    cout<<"server2"<<endl;

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