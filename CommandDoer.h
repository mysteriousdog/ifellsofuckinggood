#ifndef _COMMANDDOER_H_
#define _COMMANDDOER_H_

#include "MyEnum.h"


class CommandDoer
{
public:
    virtual void doCommand(CMD_TYPE_UINT32_ENUM cmdType)=0;
private:

};

#endif