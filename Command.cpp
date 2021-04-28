#include "Command.h"
#include "MyEnum.h"
#include <iostream>
#include <sstream>
#include <string>

void Command::doCommand(CommandDoer &cd)
{
    cd.doCommand(type);
}

std::stringstream* Command::getSeqData()
{
    std::stringstream* ss = new std::stringstream();
    *ss<<"#type:";
    *ss<<type;
    *ss<<"#";
    return ss;
}