#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "MyEnum.h"
using namespace std;

const unsigned int RECV_BUFF = 255;
const unsigned int SEND_BUFF = 255;


class Client
{

public:
    Client();
    Client(char* ip, int port);
    bool run();
    bool recvMsg();
    bool sendMsgOnce(int id, MSG_TYPE_UINT32_ENUM type, string& str);
    bool init();
    void operator () ();
    void operator () (int rcv);
private:
    int client;
    struct sockaddr_in serverAddr;
    string rcvBuff;
    string sendBuff;
    char const* ip;
    int port;
};

#endif // _CLIENT_H_