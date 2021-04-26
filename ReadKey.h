#ifndef _READKEY_
#define _READKEY_
#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include <termio.h>
#include <stdio.h>

class ReadKey : public Singleton<ReadKey>
{
public:
    int scanKeyBoard();
private:

};

#endif