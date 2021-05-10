#include "Actor.h"
#include "MyEnum.h"
#include "ReadKey.h"
#include "Command.h"
#include <iostream>
using namespace std;

Actor::Actor()
{

}

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
    if (cmd == nullptr) {
        return;
    }
    cmd->doCommand(*this);
    delete(cmd);
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