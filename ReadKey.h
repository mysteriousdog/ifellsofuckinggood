#ifndef _READKEY_
#define _READKEY_
#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include <map>
#include <queue>
#include "MyEnum.h"
class ReadKey : public Singleton<ReadKey>
{
public:
    CMD_TYPE_UINT32_ENUM scanKeyBoard();
    CMD_TYPE_UINT32_ENUM getCmd();
protected:
    ReadKey();
private:
    
    std::map<int, CMD_TYPE_UINT32_ENUM> cmd;
    std::queue<CMD_TYPE_UINT32_ENUM> cmdQue;
friend class Singleton;
};

#endif