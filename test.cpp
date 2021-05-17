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
    util.splitStr(res, s, "@");
    assert(res.size() == 2);
    assert(res[0] == "reg");
    assert(res[1] == "zhulihua|123456");
    util.splitStr(res, res[1], "|");
    assert(res.size() == 2);
    assert(res[0] == "zhulihua");
    assert(res[1] == "123456");
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