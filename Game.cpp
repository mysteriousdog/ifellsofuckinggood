#include "Game.h"
#include "ReadKey.h"
#include "MyEnum.h"
#include "HandleMsg.h"
#include "SeqToBin.h"
#include "ConcQueue.h"
#include <sys/timeb.h>
#include <iostream>
using namespace std;

long long systemtime()
{
    timeb t;
    ftime(&t);
    return t.time*1000+t.millitm;
}

Game::Game():
isRunning(true),
actor(Actor::getInstance()),
inputSysManger(InputSysManger::getInstance())
{
    
}

void Game::updateWordPerSec()
{
    
    auto lastTime = systemtime();
    while (isRunning) {
        auto currentTime = systemtime();
        auto deltaTime = currentTime - lastTime;
        if (deltaTime > 1000 / 60) {
            lastTime = currentTime;
            update();
        }
    }

}


void Game::update()
{
    ReadKey& read = ReadKey::getInstance();
    (void)read.scanKeyBoard();
    inputSysManger.handleSysInput();
    actor.update();
    SeqToBin& seq = SeqToBin::getInstance();
    TransObj* rcvObj;
    if (seq.getRcvBuff().tryAndPop(rcvObj)) {
        cout<<"-----------get recv pop beg-----------"<<endl;
        MsgHandler& msgHandler =  MsgHandler::getInstance();
	    msgHandler.handle(rcvObj, -1);
        cout<<"-----------get recv pop end-----------"<<endl;
    }
    
    // cout<<"----------"<<endl;
}