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

void fun() {
    cout<<"111"<<endl;
    // IOManger& IOManger = IOManger::getInstance();
    // IOManger();
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

class A {
public:
    void f(int b) {
        cout<<"fuck!"<<b<<endl;
    }
};

int main()
{
    // UtilTestUnit();
    // ThreadPool::getInstance().init(2);
    // // IOManger& IOManger = IOManger::getInstance();
    // ThreadPool::getInstance().enqueue(fun);
    // Game& game = Game::getInstance();
    // game();
    A a;
    ThreadPool::getInstance().init(2);
    ThreadPool::getInstance().enqueue(&A::f, a, 1);
    // ThreadPool::getInstance().enqueue(fun);
    sleep(1);
    return 0;
}