#define TEST 0
#if not TEST
#include "Zepoll.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    if (!ComManger::getInstance().init()) {
        cout<<"ComManger init err"<<endl;
        return 1;
    }
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
using namespace std;

using namespace std;

void showVec(vector<string>& response) {
    for (size_t loop = 0; loop < response.size(); loop++) {
        cout<<response[loop]<<" ";
    }
    cout<<endl;
}

int main(int argc, char** argv) {
    if (!ComManger::getInstance().init()) {
        cout<<"ComManger init err"<<endl;
        return 1;
    }

    // vector<string> response;
    // if (KGRedisClient::getInstance().ExecLpush(response, "userList", to_string(1))) {
    //     cout<<"Set redis success response is: "<<response<<endl;
    // } else {
    //     cout<<"Set redis err response is: "<<response<<endl;
    // }

	// if (KGRedisClient::getInstance().ExecHgetAll(response, "userHash")) {
    //     cout<<"Set redis success response is: "<<endl;
    //     showVec(response);
    // } else {
    //     cout<<"Set redis err response is: "<<endl;
    //     showVec(response);
    // }

	// if (KGRedisClient::getInstance().ExecRpush(response, "userList", to_string(2))) {
    //     cout<<"Set redis success response is: "<<response<<endl;
    // } else {
    //     cout<<"Set redis err response is: "<<response<<endl;
    // }
    // if (KGRedisClient::getInstance().ExecGetString(response, "userid")) {
    //     cout<<"Get redis success response is: "<<response<<endl;
    // } else {
    //     cout<<"Get redis err response is: "<<response<<endl;
    // }
	return 0;
}
#endif