#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include "Actor.h"
#include "ReadKey.h"
#include "Game.h"
#include "SeqToBin.h"
#include "Client.h"
#include "SeqAbleObj.h"
#include "Util.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include "Player.h"
#include "IOManger.h"
#include "ThreadPool.h"
#include <unistd.h>
using namespace std;

bool clientInit(size_t threadPoolSize) {
    cout<<"init client  start"<<endl;
    int ret = 0;
    ThreadPool::getInstance().init(threadPoolSize);
    Client client;
    ThreadPool::getInstance().enqueue(&Client::run, &client);
    ThreadPool::getInstance().enqueue(&Client::recvMsg, &client);
    IOManger& iomanger = IOManger::getInstance();
    ThreadPool::getInstance().enqueue(&IOManger::run, &(IOManger::getInstance()));
    sleep(1);
    cout<<"init client complete"<<endl;
    return ret;
}

void clientEnd() {
    cout<<"end client  start"<<endl;
    TransObj* obj = new TransObj(1,MSG_BUTTON,3, -1);
    sleep(3);
    SeqToBin::getInstance().getBuff().waitPushTillEmpty(obj);
    sleep(1);
    cout<<"end client complete"<<endl;
}

void UtilTestUnit() {
    Util util;
    vector<string> res;
    string s {"reg@zhulihua|123456"};
    util.splitStr(res, move(s), "@");
    assert(res.size() == 2);
    assert(res[0] == "reg");
    assert(res[1] == "zhulihua|123456");
    util.splitStr(res, move(res[1]), "|");
    assert(res.size() == 2);
    assert(res[0] == "zhulihua");
    assert(res[1] == "123456");

    TransObj* obj;

    // MSG_REG的测试
    obj = util.getMsgFromInput("regin@zhulihua|123456");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_REG);
    assert(strncmp(obj->msg, "zhulihua", NAME_MAX_LEN) == 0);
    assert(strncmp(obj->msg  + NAME_MAX_LEN, "123456", PASSWORD_MAX_LEN) == 0);
    delete(obj);

    // MSG_LOGIN的测试
    obj = util.getMsgFromInput("login@zhulihua|123456");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_LOGIN);
    assert(strncmp(obj->msg, "zhulihua", NAME_MAX_LEN) == 0);
    assert(strncmp(obj->msg  + NAME_MAX_LEN, "123456", PASSWORD_MAX_LEN) == 0);
    delete(obj);

    // MSG_LOGOUT的测试
    obj = util.getMsgFromInput("logout@zhulihua|123456");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_LOGOUT);
    delete(obj);

    // MSG_TALK的测试
    obj = util.getMsgFromInput("zhulihua wqdwq 123456");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_TALK);
    assert(strncmp(obj->msg, "zhulihua wqdwq 123456", MAX_TRANS_MSG_LEN) == 0);
    delete(obj);

    obj = util.getMsgFromInput("add@lily");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_ASK_FOR_FRIEND);
    assert(strncmp(obj->msg, "lily", NAME_MAX_LEN) == 0);
    delete(obj);

    obj = util.getMsgFromInput("chose@lily");
    assert(obj != nullptr);
    assert(strncmp(Player::getInstance().getTalkerName().c_str(), "lily", NAME_MAX_LEN) == 0);
    delete(obj);
}


int main()
{
    clientInit(4);
    Game& game =  Game::getInstance();
    game();
    clientEnd();
    return 0;
}