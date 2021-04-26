#include "Command.h"
#include "MyEnum.h"

void Command::doCommand(CommandDoer &cd)
{
    cd.doCommand(type);
}