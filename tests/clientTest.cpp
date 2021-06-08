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
#include "FileIO.h"
#include <unistd.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
// 保存为XML
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

// 保存为纯文本
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
using namespace std;

bool clientInit(size_t threadPoolSize) {
    LOG_INFO("init client  start");
    MyLog::getInstance().Init(LOG_PATH);
    LOG_INFO("init MyLog  succ");
    Player::getInstance().loadPlayerData();
    LOG_INFO("init Player  succ");
    int ret = 0;
    ThreadPool::getInstance().init(threadPoolSize);
    LOG_INFO("init ThreadPool  succ");
    Client* rcvClient = new Client("121.5.41.213", 8877);
    LOG_INFO("create rcvClient  succ");
    rcvClient->init();
    LOG_INFO("init rcvClient  succ");

    ThreadPool::getInstance().enqueue(&Client::run, rcvClient);
    ThreadPool::getInstance().enqueue(&Client::recvMsg, rcvClient);
    LOG_INFO("init client thread succ");
    ThreadPool::getInstance().enqueue(&IOManger::run, &(IOManger::getInstance()));
    LOG_INFO("init IOManger thread succ");
    sleep(1);
    LOG_INFO("init client complete");
    return ret;
}

void clientEnd() {
    LOG_INFO("end client  start");
    TransObj* obj = new TransObj(1,MSG_BUTTON,3, -1);
    sleep(3);
    SeqToBin::getInstance().getBuff().waitPushTillEmpty(obj);
    sleep(1);
    LOG_INFO("end client complete");
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

void save(string path, map<int, PlayerData> &m) {
    std::ofstream file(path);
    // binary_oarchive 会以二进制形式保存，若要改为xml或text，将头文件和该类型的“binary”直接替换为xml或者text即可
    boost::archive::binary_oarchive oa(file, boost::archive::no_header);
    oa << m;
}

void load(string path, map<int, PlayerData> &m)
{
    std::ifstream file(path);
    boost::archive::binary_iarchive ia(file, boost::archive::no_header);
    ia >> m;
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


void test_clientFileIO()
{
    string name = "zlh";
    string passwd = "123";
    int id = 1;
    Player::getInstance().init(name, passwd, id);
    Player::getInstance().addFriend("lily", 2);
    Player::getInstance().addFriend("l2ily", 3);
    Player::getInstance().savePlayerData();

    if (Player::getInstance().loadPlayerData())
    {
        cout<<Player::getInstance().getPlayerName()<<endl;
        cout<<Player::getInstance().getPlayerId()<<endl;
        auto friends = Player::getInstance().getAllFriends();
        for (auto it = friends.begin(); it != friends.end(); it++) {
            cout<<it->first<<" "<< it->second->id <<endl;
        }
    }
}

int main()
{
    clientInit(4);
    Game& game =  Game::getInstance();
    game();
    clientEnd();

    // test_clientFileIO();

    // test_clientLog();

    // test_clientReginAccHandle();

    return 0;
}

#endif

