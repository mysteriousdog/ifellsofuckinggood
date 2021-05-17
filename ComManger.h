#ifndef _COM_MANGER_H_
#define _COM_MANGER_H_

#include "Singleton.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "redis_pool.h"
using namespace std;

typedef struct RedisDataManger {
    char isUserHashChanged : 1;
    char rsv : 7;
    unordered_map<string, int> userHash;
    RedisDataManger() : isUserHashChanged(0){};
} redisDataManger;

class ComManger : public Singleton<ComManger>
{
public:

    bool init() {
        data = new redisDataManger();
        if (data == nullptr) {
            return false;
        }
        return flushAllUserHashData();
    }
    bool flushAllUserHashData() {
        vector<string> response;
        if (KGRedisClient::getInstance().ExecHgetAll(response, "userHash")) {
            cout<<"reflash redis userHash success"<<endl;
        } else {
            cout<<"reflash redis userHash err"<<endl;
            return false;
        }
        for (int loop = 0; loop < response.size(); loop += 2) {
            data->userHash[response[loop]] = atoi(response[loop + 1].c_str());
        }
        data->isUserHashChanged = 0;
        return true;
    }

    unordered_map<string, int>& getAllUserMap() {
        if (data->isUserHashChanged == 1) {
            flushAllUserHashData();
            data->isUserHashChanged = 0;
        }
        return data->userHash;
    }
    void setUserMapFlag(int _isUserHashChanged) {
        data->isUserHashChanged = _isUserHashChanged;
    }
    bool addSessionTalker(int id, string&& name, int fd);
    bool isTalkerOnline(int id);
    bool removeSessionTalker(int id);
    int getTalkerFd(int id);
private:
    ComManger(){};
    redisDataManger* data;
friend class Singleton;
};


#endif // _COM_MANGER_H_