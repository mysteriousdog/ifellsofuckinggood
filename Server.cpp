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
using namespace std;


// void showVec(vector<string>& response) {
//     for (size_t loop = 0; loop < response.size(); loop++) {
//         cout<<response[loop]<<" ";
//     }
//     cout<<endl;
// }

int main(int argc, char** argv) {
    MysqlPool::GetInstance().initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", 2);
    // MysqlPool* mysqlPool = new MysqlPool();
    // mysqlPool->initPool("tcp://127.0.0.1:3306", "root", "353656535132Zlh!", 2);
	return 0;
}
#endif