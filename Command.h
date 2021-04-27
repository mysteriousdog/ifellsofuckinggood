#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CommandDoer.h"
#include "MyEnum.h"

class Command
{
public:
    Command(CMD_TYPE_UINT32_ENUM type):type(type){}
    
    void doCommand(CommandDoer &cd);

private:
    CMD_TYPE_UINT32_ENUM type;

};

#endif