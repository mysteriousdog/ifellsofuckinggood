#ifndef _GAME_OBJ_H
#define _GAME_OBJ_H

class GameObj
{
public:
    virtual void update()=0;
    ~GameObj(){}
};

#endif