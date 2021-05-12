#include <iostream>
#include <stdexcept>  
#include <exception>  
#include <stdio.h>  
#include <stdarg.h>
#include "mysql_pool.h"
 
using namespace std;
using namespace sql;
 
pthread_mutex_t MysqlPool::lock = PTHREAD_MUTEX_INITIALIZER;
 
//Singleton: get the single object
MysqlPool& MysqlPool::GetInstance()
{  
    static MysqlPool instance_;
    cout<<"GetInstance"<<endl; 
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
        perror("Get sql driver failed");
        cout<<"Get sql driver failed"<<endl;  
    }  
    catch(std::runtime_error& e)  
    {  
        perror("Run error"); 
        cout<<"Run error"<<endl; 
    }  
    cout<<"initPool"<<endl; 
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
            perror("create conn error");
            cout<<"create conn error"<<endl;   
        }  
    }  
    pthread_mutex_unlock(&lock);  
 
}
 
Connection* MysqlPool::CreateConnection()
{
    Connection* conn;  
    try{  
        cout<<"CreateConnection"<<endl;
        conn = driver->connect(this->url,this->user,this->password);  //create a conn 
        cout<<"CreateConnection2"<<endl;
        return conn;  
    }  
    catch(sql::SQLException& e)  
    {  
        perror("link error"); 
        cout<<"link error"<<endl;   
        return NULL;  
    }  
    catch(std::runtime_error& e)  
    {  
        perror("run error");
        cout<<"run error"<<endl;   
        return NULL;  
    }  
}
 
Connection* MysqlPool::GetConnection()
{  
    Connection* conn;  
 
    pthread_mutex_lock(&lock);
 
    if(connList.size()>0)//the pool have a conn 
    {  
        conn = connList.front(); 
        connList.pop_front();//move the first conn 
        if(conn->isClosed())//if the conn is closed, delete it and recreate it
        {  
            delete conn;  
            conn = this->CreateConnection();  
        }  
 
        if(conn == NULL)  
        {  
            --curSize;  
        }
 
        pthread_mutex_unlock(&lock);
 
        return conn;  
    }  
    else
    {  
        if(curSize< maxSize)//the pool no conn
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
                return NULL;  
            }  
        }  
        else //the conn count > maxSize
        { 
            pthread_mutex_unlock(&lock);  
            return NULL;  
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
    if(conn)  
    {  
        try{  
            conn->close();  
        }  
        catch(sql::SQLException&e)  
        {  
            perror(e.what());  
        }  
        catch(std::exception& e)  
        {  
            perror(e.what());  
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
        cout<<"MysqlPool::ExecQuery err in get con "<<endl;
        return reply;
    }
    conn->setSchema(databaseName);
    char cmdBuf[100];
    Statement *state = conn->createStatement();
    va_list args;
    va_start(args, format);
    vsnprintf(cmdBuf, 100, format, args);
    va_end(args);
    cout<<"in fun MysqlPool::ExecQuery "<<cmdBuf<<endl;
    try
    {
        reply = state->executeQuery(cmdBuf);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }

    cout<<"in fun MysqlPool::ExecQuery reply is end"<<endl;
    return reply;
}