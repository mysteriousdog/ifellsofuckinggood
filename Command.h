#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CommandDoer.h"
#include "MyEnum.h"

class Command
{
public:
    Command(CmdType type):type(type){}
    
    void doCommand(CommandDoer &cd);

private:
    CmdType type;

};

#endif