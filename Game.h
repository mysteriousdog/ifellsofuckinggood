#ifndef _GAME_H_
#define _GAME_H_

#include "Singleton.h"
#include "Actor.h"

class Game : public Singleton<Game>
{
public:
    
    void updateWordPerSec();
    void update();
protected:
    Game();
private:
    
    bool isRunning;
    Actor &actor;

friend class Singleton;
};

#endif