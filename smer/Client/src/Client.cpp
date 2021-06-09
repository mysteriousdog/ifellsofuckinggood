#include "Client.h"
#include "ConcQueue.h"
#include "SeqToBin.h"
#include "SeqAbleObj.h"
#include "Command.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Log.h"
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
    LOG_INFO("Client::init start");
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        LOG_ERR("Error: socket");
        return false;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        LOG_ERR("Error: connect");
        return false;
    }
    LOG_INFO("Client success: connect");
    return true;
}

bool Client::run()
{
    SeqToBin& seq = SeqToBin::getInstance();
    char buf[MAX_LEN_OF_SEND_BUFF] = {0};
    int sendLen;
    int objSize = sizeof(TransObj);
    LOG_INFO("client thread begin !! ");
    while (true) {
        shared_ptr<TransObj> tansObj = seq.getBuff().pop();
        if (tansObj == nullptr) {
            continue;
        }
        if (tansObj->msgType == MSG_BUTTON) {
            LOG_DEBUG("quit client...");
            break;
        }
        memset(buf, 0, MAX_LEN_OF_SEND_BUFF);
        memcpy((void*)buf, (void*)(tansObj.get()), objSize);
        // memcpy((void*)((TransObj*)buf)->msg, tansObj->msg, tansObj->len);
        sendLen = objSize;
#ifdef SERVER_COMPARE
        int fd = tansObj->fd;
        if (send(fd, buf, sendLen, 0) <= 0) {
            LOG_ERR("send from server err ...");
        }

#endif
#ifdef CLIENT_COMPARE
        if (send(client, buf, MAX_LEN_OF_SEND_BUFF, 0) <= 0) {
            LOG_ERR("send from client err ...");
        }
#endif
    }

    close(client);
    return true;
}

bool Client::recvMsg()
{
    LOG_INFO("rcv msg!!! ");
    char rcvBuf[512] = {0};
    while (recv(client, rcvBuf, sizeof(rcvBuf), 0) > 0) {
        auto recvTansObj = make_shared<TransObj>();
        if (!recvTansObj->transBuff2Obj(rcvBuf)) {
            LOG_ERR("recvTansObj->transBuff2Obj err ...");
            continue;
        }
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
    run();
}

void Client::operator () (int rcv)
{
    recvMsg();
}
