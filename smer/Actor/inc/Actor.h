#ifndef _ACTOT_H_
#define _ACTOT_H_

#ifdef CLIENT_COMPARE

#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "GameObj.h"

class Actor : public CommandDoer, public Singleton<Actor>, public GameObj
{
public:
    void action();
    virtual void doCommand(CMD_TYPE_UINT32_ENUM cmdType) override;
    virtual void update() override;
protected:
    
private:
    Actor();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
friend class Singleton;
};

#endif
#endif