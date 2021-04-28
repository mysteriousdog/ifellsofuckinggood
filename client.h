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
using namespace std;

const unsigned int RECV_BUFF = 255;
const unsigned int SEND_BUFF = 255;


class Client
{

public:
    Client();
    Client(string& ip, int port);
    bool run();
    bool init();

private:
    int client;
    struct sockaddr_in serverAddr;
    string rcvBuff;
    string sendBuff;
    string ip;
    int port;
};

#endif // _CLIENT_H_