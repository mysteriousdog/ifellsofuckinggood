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
void fun2() {
    cout<<"222"<<endl;
}

void fun3() {
    cout<<"333"<<endl;
}

int main()
{
    // ThreadPool& threadPool = ThreadPool::getInstance();
    // ThreadPool* threadPool = new ThreadPool(3);
    // // TalkManger& talkManger = TalkManger::getInstance();
    // threadPool->enqueue(fun);
    // threadPool->enqueue(fun2);
    // threadPool->enqueue(fun3);
    ThreadPool* threadPool = new ThreadPool(1);
    TalkManger& talkManger = TalkManger::getInstance();
    threadPool->enqueue(fun);
    Game& game = Game::getInstance();
    game();
    return 0;
}