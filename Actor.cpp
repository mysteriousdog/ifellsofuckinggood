#include "Actor.h"
#include "MyEnum.h"
#include "ReadKey.h"
#include "Command.h"
#include <iostream>
using namespace std;

void Actor::moveUp()
{
    cout<<"i am now moving up!!"<<endl;
}

void Actor::moveDown()
{
    cout<<"i am now moving down!!"<<endl;
}

void Actor::moveLeft()
{
    cout<<"i am now moving left!!"<<endl;
}

void Actor::moveRight()
{
    cout<<"i am now moving right!!"<<endl;
}

void Actor::action()
{
    Command* cmd = ReadKey::getInstance().getCmd();
    cmd->doCommand(*this);
}

void Actor::doCommand(CMD_TYPE_UINT32_ENUM cmdType)
{
    
    switch (cmdType)
    {
    case CMD_MOVE_UP:
        moveUp();
        break;
    case CMD_MOVE_DOWN:
        moveDown();
        break;
    case CMD_MOVE_LEFT:
        moveLeft();
        break;
    case CMD_MOVE_RIGHT:
        moveRight();
        break;
    default:
        break;
    }
}

void Actor::update()
{
    action();
}