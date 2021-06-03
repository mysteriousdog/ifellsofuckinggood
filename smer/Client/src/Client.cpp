#include "Client.h"
#include "ConcQueue.h"
#include "SeqToBin.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

const int MAX_LEN_OF_SEND_BUFF = 512;

Client::Client():
client(0),
port(0)
{
}


Client::Client(const char* ip_, int port_):
ip(ip_),
port(port_)
{
}

bool Client::init()
{
    std::cout << "Client::init0" << std::endl;
    client = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "Client::init1" << std::endl;
    if (client == -1) {
        std::cout << "Error: socket" << std::endl;
        return false;
    }
    // serverAddr.sin_family = AF_INET;
    // serverAddr.sin_port = htons(port);
    // serverAddr.sin_addr.s_addr = inet_addr(ip);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    std::cout << "Client::init3" << std::endl;
    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error: connect" << std::endl;
        return false;
    }
    std::cout << "success: connect" << std::endl;
    return true;
}

bool Client::run()
{
    SeqToBin& seq = SeqToBin::getInstance();
    char buf[MAX_LEN_OF_SEND_BUFF] = {0};
    int sendLen;
    int objSize = sizeof(TransObj);
    cout<<"client thread begin !! "<<client<<endl;
    while (true) {
        cout<<"wait for event..."<<endl;
        TransObj* tansObj = seq.getBuff().pop();
        if (tansObj == nullptr) {
            continue;
        }
        if (tansObj->msgType == MSG_BUTTON) {
            cout<<"quit client..."<<endl;
            break;
        }
        memset(buf, 0, MAX_LEN_OF_SEND_BUFF);
        memcpy((void*)buf, (void*)tansObj, objSize);
        // memcpy((void*)((TransObj*)buf)->msg, tansObj->msg, tansObj->len);
        sendLen = objSize;
#ifdef SERVER_COMPARE
        cout<<"send from server msgType is ... "<<(int)tansObj->getMsgType()<<endl;
        int fd = tansObj->fd;
        cout<<"fd "<<fd<<endl;
        if (send(fd, buf, sendLen, 0) <= 0) {
            cout<<"send from server err ..."<<endl;
        }

#endif
#ifdef CLIENT_COMPARE
        cout<<"send from client ..."<<tansObj->getMsgType()<<endl;
        cout<<"send from client msg ..."<<tansObj->getMsg()<<endl;
        cout<<"send from client name ..."<<tansObj->getName()<<endl;
        cout<<"send from client Passwd ..."<<tansObj->getPasswd()<<endl;
        if (send(client, buf, MAX_LEN_OF_SEND_BUFF, 0) <= 0) {
            cout<<"send from client err ..."<<endl;
        }
#endif
        delete(tansObj);
    }

    close(client);
    return true;
}

bool Client::recvMsg()
{
    cout<<"rcv msg!!! "<<client<<endl;
    // if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    //     std::cout << "Error: connect" << std::endl;
    //     return false;
    // }
    char rcvBuf[512] = {0};
    // string s("1");
    // if (sendMsgOnce(1, MSG_CONNECT, s)) {
    //     cout<<"id band success"<<endl;
    // } else {
    //     cout<<"id band err"<<endl;
    //     return false;
    // }
    while (recv(client, rcvBuf, sizeof(rcvBuf), 0) > 0) {
        // cout<<"rcv "<<rcvBuf<<endl;
        // TransObj* recvTansObj = (TransObj*)buf;
        TransObj* recvTansObj = new TransObj();
        // recvTansObj->setFd(((TransObj*)rcvBuf)->getFd());
        // recvTansObj->setId(((TransObj*)rcvBuf)->getId());
        // recvTansObj->setMsgType(((TransObj*)rcvBuf)->getMsgType());
        // recvTansObj->setMsg(((TransObj*)rcvBuf)->getMsg());
        if (!recvTansObj->transBuff2Obj(rcvBuf)) {
            delete(recvTansObj);
            continue;
        }

        // memcpy((void*)recvTansObj, (void*)rcvBuf, sizeof(TransObj));
        std::cout << recvTansObj->id << std::endl;
        std::cout << recvTansObj->msgType << std::endl;
        std::cout << recvTansObj->len << std::endl;
        std::cout << recvTansObj->getMsg() << std::endl;
        cout << hex << (void *)(recvTansObj) << endl;
        SeqToBin::getInstance().getRcvBuff().push(recvTansObj);
    }
    close(client);
    return true;
}

bool Client::sendMsgOnce(int id, MSG_TYPE_UINT32_ENUM type, string& str)
{
    char buf[MAX_LEN_OF_SEND_BUFF];
    TransObj sendTansObj(id, type, str.length());
    sendTansObj.setMsg(str.c_str());
    memcpy((void*)buf, (void*)(&sendTansObj), sizeof(TransObj));
    if (send(client, buf, sizeof(buf), 0) > 0) {
        return true;
    }
    return false;;
}


void Client::operator () ()
{
    // ip = "121.5.41.213";
    // port = 8877;
    // init();
    run();
}

void Client::operator () (int rcv)
{
    // init();
    recvMsg();
}
