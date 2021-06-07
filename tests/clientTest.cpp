#ifdef CLIENT_COMPARE
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
#include "SysManger.h"
#include "Log.h"
#include <unistd.h>
using namespace std;

bool clientInit(size_t threadPoolSize) {
    cout<<"init client  start"<<endl;
    MyLog::getInstance().Init(LOG_PATH);
    cout<<"init MyLog  succ"<<endl;
    int ret = 0;
    ThreadPool::getInstance().init(threadPoolSize);
    cout<<"init ThreadPool  succ"<<endl;

    Client* rcvClient = new Client("121.5.41.213", 8877);
    cout<<"create rcvClient  succ"<<endl;
    rcvClient->init();
    cout<<"init rcvClient  succ"<<endl;

    // Client* sendClient = new Client("121.5.41.213", 8877);
    // cout<<"create sendClient  succ"<<endl;
    // sendClient->init();
    // cout<<"init sendClient  succ"<<endl;

    ThreadPool::getInstance().enqueue(&Client::run, rcvClient);
    ThreadPool::getInstance().enqueue(&Client::recvMsg, rcvClient);
    cout<<"init client thread succ"<<endl;
    ThreadPool::getInstance().enqueue(&IOManger::run, &(IOManger::getInstance()));
    cout<<"init IOManger thread succ"<<endl;
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


void test_clientReginAccHandle()
{
    TransObj* obj = new TransObj();
    obj->setName("zlh");
    obj->setPasswd("123");
    obj->setId(48);
    obj->setMsgType(MSG_LOGIN_ACCEPT);
    SeqToBin::getInstance().getRcvBuff().push(obj);
    SysManger::getInstance().handleRecvMsg();
}

void test_clientLog()
{
    MyLog::getInstance().Init(LOG_PATH);
    MyLog::getInstance().Log("test");
    MyLog::getInstance().Log("test2");
    LOG_DEBUG("test3");
    LOG_INFO("test4");
    LOG_ERR("test5");
}


int main()
{
    // clientInit(4);
    // Game& game =  Game::getInstance();
    // game();
    // clientEnd();
    test_clientLog();

    // test_clientReginAccHandle();

    return 0;
}

#endif

