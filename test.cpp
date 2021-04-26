#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include "Actor.h"
#include "ReadKey.h"
#include <iostream>
using namespace std;

int main()
{
    // Actor& actor = Actor::getInstance();
    // Command *cmd = new Command(CmdType::CMD_MOVE_UP);
    // cmd->doCommand(actor);

    auto read = ReadKey::getInstance();
    while (1) {
        cout<<read.scanKeyBoard()<<endl;
    }
    return 0;
}