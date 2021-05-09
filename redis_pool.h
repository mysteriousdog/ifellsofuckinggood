#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <mutex>
#include <queue>
#include <sys/time.h>
#include "hiredis/hiredis.h"
class KGRedisClient
{
public:
    KGRedisClient(std::string ip, int port, std::string password, int timeout = 2000);
    virtual ~KGRedisClient();
 //   bool ExecuteCmd_spop(const char *cmd, size_t len, std::string &response);
    bool ExecuteCmd_spop(std::string &response, const char* format, ...);
 
 //   redisReply* ExecuteCmd(const char *cmd, size_t len);
    redisReply* ExecuteCmd(const char* format, ...);
private:
    int m_timeout;
    int m_serverPort;
    std::string m_setverIp;
    std::string m_password;
 //   CCriticalSection m_lock;
    std::mutex _mutex;
    std::queue<redisContext *> m_clients;
    time_t m_beginInvalidTime;
    static const int m_maxReconnectInterval = 3;
    redisContext* CreateContext();
    void ReleaseContext(redisContext *ctx, bool active);
    bool CheckStatus(redisContext *ctx);
};

#endif // __REDIS_POOL_H__