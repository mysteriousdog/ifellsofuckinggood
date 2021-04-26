#ifndef _COMMANDDOER_H_
#define _COMMANDDOER_H_

#include "MyEnum.h"


class CommandDoer
{
public:
    virtual void doCommand(CmdType& cmdType)=0;
private:

};

#endif