#ifndef _MYSQL_POOL_H_
#define _MYSQL_POOL_H_

#ifdef SERVER_COMPARE
#include <iostream>
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include <cppconn/exception.h>  
#include <cppconn/driver.h>  
#include <cppconn/connection.h>  
#include <cppconn/resultset.h>  
#include <cppconn/prepared_statement.h>  
#include <cppconn/statement.h>  
#include <pthread.h>  
#include <list>
#include <vector>  
 
using namespace std;  
using namespace sql; 
 
class MysqlPool
{
public:
    // Singleton 
    static MysqlPool& GetInstance();
 
    //init pool
    void initPool(std::string url_, std::string user_, std::string password_, int maxSize_, string&& databaseName);
 
    //get a conn from pool
    Connection* GetConnection();
 
    //put the conn back to pool
    void ReleaseConnection(Connection *conn);

    // for user to do query thing like ExecQuery("select * from userinfo where id = %d", 1);
    ResultSet* ExecQuery(const char* format, ...); 

    // for user to do insert thing like ExecInsert("select * from userinfo where id = %d", 1);
    bool ExecInsert(const char* format, ...);  
 
    ~MysqlPool();
    MysqlPool(){}
private:
    
 
    //init DB pool
    void InitConnection(int initSize);
    
    // create a connection
    Connection* CreateConnection(); 
 
    //destory connection
    void DestoryConnection(Connection *conn);
 
    //destory db pool
    void DestoryConnPool();

      
 
private:
    string user;
    string password;
    string url;
    string databaseName;
    int maxSize;
    int curSize;
 
    Driver*             driver;     //sql driver (the sql will free it)
    list<Connection*>   connList;   //create conn list
    //thread lock mutex
    static pthread_mutex_t lock;     
};

#endif
#endif // _MYSQL_POOL_H_
