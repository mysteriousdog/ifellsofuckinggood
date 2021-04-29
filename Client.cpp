#include "Client.h"
#include "ConcQueue.h"
#include "SeqToBin.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include <iostream>
using namespace std;
Client::Client():
client(0),
port(0)
{
}


Client::Client(string& ip_, int port_):
ip(ip_),
port(port_)
{
}

bool Client::init()
{
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        std::cout << "Error: socket" << std::endl;
        return false;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    return true;
}

bool Client::run()
{
    // if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    //     std::cout << "Error: connect" << std::endl;
    //     return false;
    // }
    SeqToBin& seq = SeqToBin::getInstance();
    // char buf[255];
    // while (true) {
    //     TransObj* tansObj = seq.buff.pop();
    //     if (tansObj == nullptr) {
    //         continue;
    //     }
    //     char* data = (char*)sendBuff.data();
    //     send(client, data, strlen(data), 0);
    //     if (strcmp(data, "exit") == 0) {
    //         std::cout << "...disconnect" << std::endl;
    //         break;
    //     }
    //     memset(buf, 0, sizeof(buf));
    //     int len = recv(client, buf, sizeof(buf), 0);
    //     buf[len] = '\0';
    //     std::cout << buf << std::endl;
    // }
    TransObj* tansObj = seq.getBuff().pop();
    if (tansObj == nullptr) {
        cout<<"client get data null"<<endl;
        return false;
    } else {
        Command* cmd = (Command*)tansObj->msg;
        if (cmd == nullptr) {
            cout<<"cmd transform get data null"<<endl;
            return false;
        }
        cout<<"cmd get "<<cmd->getType()<<endl;
    }
    // close(client);
    return true;
}