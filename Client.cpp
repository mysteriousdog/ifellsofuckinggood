#include "Client.h"
#include "ConcQueue.h"
#include "SeqToBin.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
Client::Client():
client(0),
port(0)
{
}


Client::Client(char* ip_, int port_):
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
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    return true;
}

bool Client::run()
{
    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error: connect" << std::endl;
        return false;
    }
    SeqToBin& seq = SeqToBin::getInstance();
    char buf[255];
    char buf2[255];
    int sendLen;
    int objSize = sizeof(TransObj);
    cout<<"client thread begin !!"<<endl;
    while (true) {
        cout<<"wait for event..."<<endl;
        TransObj* tansObj = seq.getBuff().pop();
        if (tansObj == nullptr) {
            continue;
        }
        
        memcpy((void*)buf, tansObj, objSize);
        // memcpy((void*)((TransObj*)buf)->msg, tansObj->msg, tansObj->len);
        sendLen = objSize;
        send(client, buf, sendLen, 0);
        delete(tansObj);
        // if (strcmp(buf, "exit") == 0) {
        //     std::cout << "...disconnect" << std::endl;
        //     break;
        // }
        memset(buf2, 0, sizeof(buf2));
        int len = recv(client, buf2, sizeof(buf2), 0);
        TransObj* recvTansObj = (TransObj*)buf2;
        std::cout << recvTansObj->id << std::endl;
        std::cout << recvTansObj->msgType << std::endl;
        std::cout << recvTansObj->len << std::endl;
        Command* cmd = (Command*)recvTansObj->msg;
        std::cout << cmd->getType() << std::endl;
        if (cmd->getType() == CMD_MOVE_DOWN) {
            cout<<"out"<<endl;
            break;
        }
    }
    // TransObj* tansObj = seq.getBuff().pop();
    // char buff[255];
    // memcpy((void*)buff, tansObj, sizeof(TransObj) + tansObj->len);
    // TransObj* tansObj2 = (TransObj*)buff;
    // cout<<tansObj2->msgType<<endl;
    // cout<<tansObj2->len<<endl;
    // Command* cmd = (Command*)tansObj2->msg;
    // cout<<"cmd get "<<cmd->getType()<<endl;
    // if (tansObj == nullptr) {
    //     cout<<"client get data null"<<endl;
    //     return false;
    // } else {
    //     Command* cmd = (Command*)tansObj->msg;
    //     if (cmd == nullptr) {
    //         cout<<"cmd transform get data null"<<endl;
    //         return false;
    //     }
    //     cout<<"cmd get "<<cmd->getType()<<endl;
    // }
    // close(client);
    close(client);
    return true;
}


void Client::operator () ()
{
    // ip = "121.5.41.213";
    // port = 8877;
    init();
    run();
}