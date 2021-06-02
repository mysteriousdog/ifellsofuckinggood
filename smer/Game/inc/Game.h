#ifndef _GAME_H_
#define _GAME_H_

#include "Singleton.h"
#include "Actor.h"
#include "SysManger.h"

class Game : public Singleton<Game>
{
public:
    void operator () () {
        updateWordPerSec();
    }
    void updateWordPerSec();
    void update();
protected:
    Game();
private:
    
    bool isRunning;
    Actor &actor;
    SysManger& inputSysManger;

friend class Singleton;
};

#endif