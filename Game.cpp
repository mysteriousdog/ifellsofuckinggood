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
inputSysManger(SysManger::getInstance())
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
    cout<<"-------------"<<endl;
    inputSysManger.handleSystem();
    (void)ReadKey::getInstance().scanKeyBoard();
    actor.update();
}