/*
目前不考虑链接type的改变处理
*/


#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__
#ifdef SERVER_COMPARE
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <mutex>
#include <queue>
#include <sys/time.h>
#include "hiredis/hiredis.h"
#include "rediscluster/rediscluster.h"
#include "Singleton.h"

using namespace std;

typedef enum RedisConTYpe {
   REDIS_CON_NORMAL_TYPE,
   REDIS_CON_CLUSTER_TYPE,
   REDIS_CON_TYPE_BUTTON
}redisConType;

typedef int REDIS_CON_INT32_ENUM_TYPE;


class KGRedisClient : public Singleton<KGRedisClient>
{
public:
    
    virtual ~KGRedisClient();
 //   bool ExecuteCmd_spop(const char *cmd, size_t len, std::string &response);
    bool ExecuteCmd_spop(std::vector<std::string>& response, const char* format, ...);

    bool ExecDel(std::vector<std::string>& response, string&& key);

    bool ExecSetString(std::vector<std::string>& response, string&& strName, string&& str);

    bool ExecGetString(std::vector<std::string>& response, string&& strName);

    bool ExecLpush(std::vector<std::string>& response, string&& lstName, string&& item);

    bool ExecRpush(std::vector<std::string>& response, string&& lstName, string&& item);

    bool ExecLpop(std::vector<std::string>& response, string&& lstName);

    bool ExecRpop(std::vector<std::string>& response, string&& lstName);

    bool ExecGetLstAll(std::vector<std::string>& response, string&& lstName);

    bool ExecSadd(std::vector<std::string>& response, string&& setName, string&& val);

    bool ExecScontain(std::vector<std::string>& response, string&& setName, string&& val);

    bool ExecSremove(std::vector<std::string>& response, string&& setName, string&& val);

    bool ExecHset(std::vector<std::string>& response, string&& hashName, string&& key, string&& value);

    bool ExecHMset(std::vector<std::string>& response, string&& hashName, vector<pair<string, string>>& keyVals);

    bool ExecHget(std::vector<std::string>& response, string&& hashName, string&& key);

    bool ExecHgetAll(std::vector<std::string>& response, string&& hashName);
 
 //   redisReply* ExecuteCmd(const char *cmd, size_t len);
    redisReply* ExecuteCmd(const char* format, ...);

    void changeConType(REDIS_CON_INT32_ENUM_TYPE _conType) {
      conType = _conType;
    }
    REDIS_CON_INT32_ENUM_TYPE getConType() {
      return conType;
    }
private:
    KGRedisClient(std::string ip, int port, std::string password, int timeout = 2000);
    KGRedisClient():m_setverIp("121.5.41.213"), m_serverPort(6379), m_password("353656535132Zlh!"), m_timeout(2000), conType(REDIS_CON_CLUSTER_TYPE){cout<<"client init"<<endl;};
    int m_timeout;
    int m_serverPort;
    REDIS_CON_INT32_ENUM_TYPE conType;
    std::string m_setverIp;
    std::string m_password;
 //   CCriticalSection m_lock;
    std::mutex _mutex;
    std::queue<redisContext *> m_clients;

    std::mutex _clusterMutex;
    std::queue<redisClusterContext *> m_clusterClients;
    time_t m_beginInvalidTime;
    static const int m_maxReconnectInterval = 3;
    redisContext* CreateContext();
    redisClusterContext* CreateClusterContext();
    void ReleaseContext(redisContext *ctx, bool active);
    void ReleaseClusterContext(redisClusterContext *ctx, bool active);
    bool CheckStatus(redisContext *ctx);
    char cmdBuf[100];

friend class Singleton;
};


#endif
#endif // __REDIS_POOL_H__