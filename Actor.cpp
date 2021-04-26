#include "Actor.h"
#include "MyEnum.h"
#include <iostream>
using namespace std;

void Actor::moveUp()
{
    cout<<"i am now moving up!!"<<endl;
}

void Actor::doCommand(CmdType& cmdType)
{
    switch (int(cmdType))
    {
    case int(CmdType::CMD_MOVE_UP):
        moveUp();
        /* code */
        break;
    
    default:
        break;
    }
}