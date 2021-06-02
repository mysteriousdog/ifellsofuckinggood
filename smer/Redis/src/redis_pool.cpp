#ifdef SERVER_COMPARE
#include "redis_pool.h"
#include "ComManger.h"
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <sstream>
using namespace std;
KGRedisClient::KGRedisClient(std::string ip, int port, std::string password, int timeout)
{
    m_timeout = timeout;
    m_serverPort = port;
    m_setverIp = ip;
    m_password = password;
    m_beginInvalidTime = 0;
}
KGRedisClient::~KGRedisClient()
{
//    CAutoLock autolock(m_lock);
    std::unique_lock <std::mutex> lck(_mutex);
    while(!m_clients.empty())
    {
        redisContext *ctx = m_clients.front();
        redisFree(ctx);
        m_clients.pop();
    }
}

bool KGRedisClient::ExecuteCmd_spop(std::vector<std::string>& response, const char* format, ...)
{
    va_list args;
    redisReply* reply = nullptr;
    va_start(args, format);
    memset(cmdBuf, 0, sizeof(cmdBuf));
    vsnprintf(cmdBuf, 100, format, args);
    cout<<"format 1 "<<endl;
    cout<<cmdBuf<<endl;
    cout<<endl<<"format 2 "<<endl;
    if (conType == REDIS_CON_NORMAL_TYPE) {
        redisContext *ctx = CreateContext();
        if(ctx == NULL) {
            cout<<"CreateContext err!"<<endl;
            return false;
        }
        reply = (redisReply*)redisCommand(ctx, cmdBuf);
        ReleaseContext(ctx, reply != NULL);
    } else if (conType == REDIS_CON_CLUSTER_TYPE) {
        redisClusterContext *ctx = CreateClusterContext();
        if(ctx == NULL) {
            cout<<"CreateContext err!"<<endl;
            return false;
        }
        reply = (redisReply*)redisClusterCommand(ctx, cmdBuf);
        ReleaseClusterContext(ctx, reply != NULL);
    } else {
        cout<<"conType err!"<<endl;
        return false;
    }
    va_end(args);
    
    if(reply == NULL) {
        cout<<"reply == NULL"<<endl;
        return false;
    }
    std::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
#if 1

    if(reply->type == REDIS_REPLY_INTEGER)
    {
        response.push_back(std::to_string(reply->integer));
        return true;
    }
    else if(reply->type == REDIS_REPLY_STRING)
    {
        std::string res;
        res.assign(reply->str, reply->len);
        response.push_back(res);
        return true;
    }
    else if(reply->type == REDIS_REPLY_STATUS)
    {
        std::string res;
        res.assign(reply->str, reply->len);
        response.push_back(res);
        return true;
    }
    else if(reply->type == REDIS_REPLY_NIL)
    {
        response.push_back("");
        return true;
    }
    else if(reply->type == REDIS_REPLY_ERROR)
    {
        std::string res;
        res.assign(reply->str, reply->len);
        response.push_back(res);
        return true;
    }
    else if(reply->type == REDIS_REPLY_ARRAY)
    {
        for (size_t loop = 0; loop < reply->elements; loop++) {
            if (reply->element[loop]->type == REDIS_REPLY_INTEGER) {
                response.emplace_back(std::to_string(reply->element[loop]->integer)); 
            } else {
                response.emplace_back(reply->element[loop]->str); 
            }
        }
        return true;
    }
    else
    {
        response.push_back("Undefine Reply Type");
        return false;
    }
#endif
}

bool KGRedisClient::ExecDel(std::vector<std::string>& response, string&& key)
{
    return ExecuteCmd_spop(response, "DEL %s", key.c_str());
}

redisReply* KGRedisClient::ExecuteCmd(const char* format, ...)
{
    va_list args;
    va_start(args, format);
 
    redisContext *ctx = CreateContext();
    if(ctx == NULL) {
        cout<<"CreateContext err!"<<endl;
        return NULL;
    }
  //  redisReply *reply = (redisReply*)redisCommand(ctx, "spop %b", cmd, len);
 //   redisReply *reply = (redisReply*)redisCommand(ctx, "%s", cmd); 
    cout<<"exce1"<<endl;
    vprintf(format, args);
    memset(cmdBuf, 0, sizeof(cmdBuf));
    vsnprintf(cmdBuf, 100, format, args);
    cout<<"exce2"<<endl;
    // redisReply* reply = (redisReply*)redisvCommand(ctx, format, args);
    redisReply* reply = (redisReply*)redisCommand(ctx, cmdBuf);
    va_end(args);
 
    ReleaseContext(ctx, reply != NULL);
    return reply;
}
 

redisClusterContext* KGRedisClient::CreateClusterContext()
{

    {
//        CAutoLock autolock(m_lock);
        std::unique_lock <std::mutex> lck(_clusterMutex);
        if(!m_clusterClients.empty())
        {
            redisClusterContext *ctx = m_clusterClients.front();
            m_clusterClients.pop();
            return ctx;
        }
    }

    time_t now = time(NULL);
    if(now < m_beginInvalidTime + m_maxReconnectInterval) return NULL;
    struct timeval tv;
    tv.tv_sec = m_timeout / 1000;
    tv.tv_usec = (m_timeout % 1000) * 1000;;

    redisClusterContext *ctx = redisClusterContextInit();

    redisClusterSetOptionAddNodes(ctx, "121.5.41.213:6379,106.54.163.125");

    redisClusterSetOptionPassword(ctx, m_password.c_str());

    redisClusterSetOptionConnectTimeout(ctx, tv);

    redisClusterConnect2(ctx);

    if(ctx == NULL || ctx->err != 0)
    {
        printf("Error in cluster conn: %s\n", ctx->errstr);
        if(ctx != NULL) redisClusterFree(ctx);
        m_beginInvalidTime = time(NULL);
        return NULL;
    }

    // redisReply *reply;
    // std::string strReply = "AUTH ";
    // strReply += m_password;
    // reply = (redisReply*)redisCommand(ctx, strReply.c_str());
    // freeReplyObject(reply);
    // reply = NULL;
    printf("connect OK\n");
    return ctx;
}


redisContext* KGRedisClient::CreateContext()
{
    {
//        CAutoLock autolock(m_lock);
        std::unique_lock <std::mutex> lck(_mutex);
        if(!m_clients.empty())
        {
            redisContext *ctx = m_clients.front();
            m_clients.pop();
            return ctx;
        }
    }
    time_t now = time(NULL);
    if(now < m_beginInvalidTime + m_maxReconnectInterval) return NULL;
    struct timeval tv;
    tv.tv_sec = m_timeout / 1000;
    tv.tv_usec = (m_timeout % 1000) * 1000;;
    redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);

    if(ctx == NULL || ctx->err != 0)
    {
        if(ctx != NULL) redisFree(ctx);
        m_beginInvalidTime = time(NULL);
        return NULL;
    }
    redisReply *reply;
    std::string strReply = "AUTH ";
    strReply += m_password;
    reply = (redisReply*)redisCommand(ctx, strReply.c_str());
    freeReplyObject(reply);
    reply = NULL;
    printf("connect OK\n");
    return ctx;
}

void KGRedisClient::ReleaseContext(redisContext *ctx, bool active)
{
    if(ctx == NULL) return;
    if(!active) {redisFree(ctx); return;}
//    CAutoLock autolock(m_lock);
    std::unique_lock <std::mutex> lck(_mutex);
    m_clients.push(ctx);
}

void KGRedisClient::ReleaseClusterContext(redisClusterContext *ctx, bool active)
{
    if(ctx == NULL) return;
    if(!active) {redisClusterFree(ctx); return;}
//    CAutoLock autolock(m_lock);
    std::unique_lock <std::mutex> lck(_clusterMutex);
    m_clusterClients.push(ctx);
}

bool KGRedisClient::CheckStatus(redisContext *ctx)
{
    redisReply *reply = (redisReply*)redisCommand(ctx, "ping");
    if(reply == NULL) return false;
    std::shared_ptr<redisReply> autoFree(reply, freeReplyObject);
    if(reply->type != REDIS_REPLY_STATUS) return false;
    if(strcasecmp(reply->str,"PONG") != 0) return false;
    return true;
}

bool KGRedisClient::ExecSetString(std::vector<std::string>& response, std::string&& strName, string&& str)
{
    return ExecuteCmd_spop(response, "set %s %s", strName.c_str(), str.c_str());
}

bool KGRedisClient::ExecGetString(std::vector<std::string>& response, string&& strName)
{
    return ExecuteCmd_spop(response, "get %s", strName.c_str());
}

bool KGRedisClient::ExecLpush(std::vector<std::string>& response, string&& lstName, string&& item)
{
    return ExecuteCmd_spop(response, "lpush %s %s", lstName.c_str(), item.c_str());
}

bool KGRedisClient::ExecRpush(std::vector<std::string>& response, string&& lstName, string&& item)
{
    return ExecuteCmd_spop(response, "rpush %s %s", lstName.c_str(), item.c_str());
}

bool KGRedisClient::ExecLpop(std::vector<std::string>& response, string&& lstName)
{
    return ExecuteCmd_spop(response, "lpop %s", lstName.c_str());
}

bool KGRedisClient::ExecRpop(std::vector<std::string>& response, string&& lstName)
{
    return ExecuteCmd_spop(response, "rpop %s", lstName.c_str());
}

bool KGRedisClient::ExecGetLstAll(std::vector<std::string>& response, string&& lstName)
{
    return ExecuteCmd_spop(response, "lrange %s 0 -1", lstName.c_str());
}

bool KGRedisClient::ExecHset(std::vector<std::string>& response, string&& hashName, string&& key, string&& value)
{
    if( ExecuteCmd_spop(response, "hset %s %s %s", hashName.c_str(), key.c_str(), value.c_str())) {
        ComManger::getInstance().setUserMapFlag(1);
        return true;
    }
    return false;
}

bool KGRedisClient::ExecHMset(std::vector<std::string>& response, string&& hashName, vector<pair<string, string>>& keyVals)
{
    stringstream ss;
    ss<<"HMSET";
    ss<<" ";
    ss<<hashName;
    for (auto it = keyVals.begin(); it != keyVals.end(); it++) {
        ss<<" ";
        ss<<(*it).first;
        ss<<" ";
        ss<<(*it).second;
    }
    cout<<"KGRedisClient::ExecHMset "<<ss.str().c_str()<<endl;
    return ExecuteCmd_spop(response, ss.str().c_str());
}

bool KGRedisClient::ExecHget(std::vector<std::string>& response, string&& hashName, string&& key)
{
    return ExecuteCmd_spop(response, "hget %s %s", hashName.c_str(), key.c_str());
}

bool KGRedisClient::ExecHgetAll(std::vector<std::string>& response, string&& hashName)
{
    return ExecuteCmd_spop(response, "hgetAll %s", hashName.c_str());
}

bool KGRedisClient::ExecSadd(std::vector<std::string>& response, string&& setName, string&& val)
{
    return ExecuteCmd_spop(response, "SADD %s %s", setName.c_str(), val.c_str());
}

bool KGRedisClient::ExecScontain(std::vector<std::string>& response, string&& setName, string&& val)
{
    if (ExecuteCmd_spop(response, "SISMEMBER %s %s", setName.c_str(), val.c_str())) {
        try
        {
            if (response[0] == "1") {
                return true;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }
    return false;
}

bool KGRedisClient::ExecSremove(std::vector<std::string>& response, string&& setName, string&& val)
{
    return ExecuteCmd_spop(response, "SREM %s %s", setName.c_str(), val.c_str());
}

#endif