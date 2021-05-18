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
#include "TalkManger.h"
#include "ThreadPool.h"
#include <unistd.h>
using namespace std;

void fun() {
    cout<<"111"<<endl;
    TalkManger& talkManger = TalkManger::getInstance();
    talkManger();
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
    delete(obj);

    obj = util.getMsgFromInput("add@zhulihua|123456");
    assert(obj != nullptr);
    assert(obj->msgType == MSG_ASK_FOR_FRIEND);
    delete(obj);
}

int main()
{
    UtilTestUnit();
    // ThreadPool::getInstance().init(2);
    // // TalkManger& talkManger = TalkManger::getInstance();
    // ThreadPool::getInstance().enqueue(fun);
    // Game& game = Game::getInstance();
    // game();
    return 0;
}