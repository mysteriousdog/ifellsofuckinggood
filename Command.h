#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "CommandDoer.h"
#include "MyEnum.h"
#include "SeqAbleObj.h"

class Command : public SeqAbleObj
{
public:
    Command(CMD_TYPE_UINT32_ENUM type_):type(type_){}
    
    void doCommand(CommandDoer &cd);
    virtual std::stringstream* getSeqData() override;

private:
    CMD_TYPE_UINT32_ENUM type;



};

#endif