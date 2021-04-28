#include "client.h"
#include "ConcQueue.h"
#include "SeqToBin.h"

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
    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error: connect" << std::endl;
        return false;
    }
    SeqToBin& seq = SeqToBin::getInstance();
    char buf[255];
    while (true) {
        seq.buff.pop(sendBuff);
        char* data = (char*)sendBuff.data();
        send(client, data, strlen(data), 0);
        if (strcmp(data, "exit") == 0) {
            std::cout << "...disconnect" << std::endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
        int len = recv(client, buf, sizeof(buf), 0);
        buf[len] = '\0';
        std::cout << buf << std::endl;
    }
    close(client);
    return true;
}