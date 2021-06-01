#include "Command.h"
#include "MyEnum.h"
#include <iostream>
#include <sstream>
#include <string.h>

void Command::doCommand(CommandDoer &cd)
{
    cd.doCommand(type);
}

tansObj* Command::getSeqData()
{
    tansObj* obj = new tansObj(MSG_CMD, sizeof(*this));
    memcpy(obj->msg, (void*)this, obj->len);
    return obj;
}