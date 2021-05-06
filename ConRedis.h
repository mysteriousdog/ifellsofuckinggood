#ifndef _CONREDIS_H_
#define _CONREDIS_H_

#include <hiredis/hiredis.h>
#include <iostream>

using namespace std;

class ConRedis
{

public:
    ConRedis(){};
    ConRedis(char* ip_, int port_): ip(ip_), port(port_), c() {};
    bool conn() {
        c = redisConnect(ip,port);
        if (c->err) {
            redisFree(c);
            cout<<"connect failed !"<<endl;
            return false;
        }
        cout<<" hello linux, connect succes!"<<endl;
        return true;
    }

    void operator () () {
        
    }

    char* doCommd(string& cmd) {
        redisReply *r = (redisReply*)redisCommand(c, "auth 353656535132Zlh!");
        r = (redisReply*)redisCommand(c, cmd.c_str());
        cout<<r->str<<endl;
        return r->str;
    }
private:
    char* ip;
    int port;
    redisContext *c;
};


#endif //_CONREDIS_H_