#ifndef _ACTOT_H_
#define _ACTOT_H_

#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"

class Actor : public CommandDoer, public Singleton<Actor>
{
public:
    virtual void doCommand(CmdType& cmdType);
private:
    void moveUp();

};

#endif