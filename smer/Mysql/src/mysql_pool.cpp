#ifdef SERVER_COMPARE
#include <iostream>
#include <stdexcept>  
#include <exception>  
#include <stdio.h>  
#include <stdarg.h>
#include "mysql_pool.h"
#include "Log.h"

using namespace std;
using namespace sql;
 
pthread_mutex_t MysqlPool::lock = PTHREAD_MUTEX_INITIALIZER;
 
MysqlPool& MysqlPool::GetInstance()
{  
    static MysqlPool instance_;
    LOG_DEBUG("GetInstance"); 
    return instance_;  
}
 
void MysqlPool::initPool(std::string url_, std::string user_, std::string password_, int maxSize_, string&& databaseName)
{
    this->user = user_;  
    this->password = password_;
    this->url = url_;
    this->maxSize = maxSize_;  
    this->curSize = 0; 
    this->databaseName = databaseName;
 
    try{  
        this->driver=sql::mysql::get_driver_instance();
    }  
    catch(sql::SQLException& e)  
    {  
        LOG_ERR("Get sql driver failed"); 
    }  
    catch(std::runtime_error& e)  
    {  
        LOG_ERR("Run error");
    }  
    this->InitConnection(maxSize/2);
} 
 
//init conn pool
void MysqlPool::InitConnection(int initSize)
{
    Connection* conn;  
    pthread_mutex_lock(&lock);  
    for(int i =0;i <initSize; i++)  
    {  
        conn= this->CreateConnection();  
        
        if(conn)
        {  
            connList.push_back(conn);
            ++(this->curSize);  
        }  
        else  
        {   
            LOG_ERR("create conn error"); 
        }  
    }  
    pthread_mutex_unlock(&lock);  
 
}
 
Connection* MysqlPool::CreateConnection()
{
    Connection* conn;  
    try{  
        LOG_INFO("CreateConnection start");
        conn = driver->connect(this->url,this->user,this->password);  //create a conn 
        LOG_INFO("CreateConnection end");
        return conn;  
    }  
    catch(sql::SQLException& e)  
    {  
        LOG_ERR("Link error in MysqlPool::CreateConnection");  
        return nullptr;  
    }  
    catch(std::runtime_error& e)  
    {    
        LOG_ERR("un error in MysqlPool::CreateConnection");   
        return nullptr;  
    }  
}
 
Connection* MysqlPool::GetConnection()
{  
    Connection* conn;  
 
    pthread_mutex_lock(&lock);
 
    if(connList.size() > 0)//the pool have a conn 
    {  
        conn = connList.front(); 
        connList.pop_front();//move the first conn 
        if(conn->isClosed())//if the conn is closed, delete it and recreate it
        {  
            delete conn;  
            conn = this->CreateConnection();  
        }  
 
        if(conn == nullptr)  
        {  
            --curSize; 
        }
 
        pthread_mutex_unlock(&lock);
 
        return conn;  
    }  
    else
    {  
        if(curSize < maxSize)//the pool no conn
        {
            conn = this->CreateConnection();  
            if(conn)
            {  
                ++curSize;  
                pthread_mutex_unlock(&lock);  
                return conn;  
            }  
            else
            {  
                pthread_mutex_unlock(&lock);  
                return nullptr;  
            }  
        }  
        else //the conn count > maxSize
        { 
            pthread_mutex_unlock(&lock);  
            return nullptr;  
        }  
    }  
}  
 
//put conn back to pool
void MysqlPool::ReleaseConnection(Connection *conn)
{
    if(conn)
    {  
        pthread_mutex_lock(&lock);  
        connList.push_back(conn);  
        pthread_mutex_unlock(&lock);  
    }  
}
 
void MysqlPool::DestoryConnPool()
{  
    list<Connection*>::iterator iter;  
    pthread_mutex_lock(&lock);  
    for(iter = connList.begin(); iter!=connList.end(); ++iter)  
    {  
        this->DestoryConnection(*iter); 
    }  
    curSize=0;  
    connList.clear();
    pthread_mutex_unlock(&lock);  
}  
 
 
void MysqlPool::DestoryConnection(Connection* conn)  
{  
    LOG_INFO("MysqlPool::DestoryConnection");
    if(conn)  
    {  
        try{  
            conn->close();  
        }  
        catch(sql::SQLException&e)  
        {  
            LOG_ERR(e.what()); 
        }  
        catch(std::exception& e)  
        {  
            LOG_ERR(e.what());  
        } 
        delete conn;  
    }  
} 
 
MysqlPool::~MysqlPool()  
{  
    this->DestoryConnPool();  
}

ResultSet* MysqlPool::ExecQuery(const char* format, ...)
{
    ResultSet* reply = nullptr;
    Connection *conn = GetConnection();
    if (conn == nullptr) {
        LOG_INFO("MysqlPool::ExecQuery err in get con ");
        return reply;
    }
    conn->setSchema(databaseName);
    char cmdBuf[200];
    Statement *state = conn->createStatement();
    va_list args;
    va_start(args, format);
    vsnprintf(cmdBuf, 100, format, args);
    va_end(args);
    try
    {
        reply = state->executeQuery(cmdBuf);
    }
    catch(const std::exception& e)
    {
        LOG_INFO("MysqlPool::ExecQuery" + e.what());
        state->close();
        delete(state);
        ReleaseConnection(conn);
        return nullptr;
    }
    state->close();
    delete(state);
    ReleaseConnection(conn);
    return reply;
}

bool MysqlPool::ExecInsert(const char* format, ...)
{
    ResultSet* reply = nullptr;
    Connection *conn = GetConnection();
    if (conn == nullptr) {
        LOG_INFO("MysqlPool::ExecInsert err in get con ");
        return reply;
    }
    conn->setSchema(databaseName);
    char cmdBuf[200];
    va_list args;
    va_start(args, format);
    vsnprintf(cmdBuf, 100, format, args);
    va_end(args);

    bool res = false;;
    try
    {
        auto prep_stmt = conn->prepareStatement(cmdBuf);
        if (prep_stmt->executeUpdate() > 0) {
            res = true;
        }
    }
    catch(const std::exception& e)
    {
        LOG_INFO("executeUpdate"  +  e.what());
        ReleaseConnection(conn);
        return false;
    }
    ReleaseConnection(conn);
    return res;
}

#endif