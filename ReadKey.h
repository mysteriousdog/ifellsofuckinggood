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
    Command* getCmd();
    Command* getSysCmd();
protected:
    ReadKey();
private:
    bool isTimeToReadKey();
    bool isActorCmd(CMD_TYPE_UINT32_ENUM type);
    bool isSystemCmd(CMD_TYPE_UINT32_ENUM type);
    std::map<int, CMD_TYPE_UINT32_ENUM> cmd;
    std::queue<Command*> cmdQue;
    std::queue<Command*> sysCmdQue;
friend class Singleton;
};

#endif