#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include "Actor.h"
#include "ReadKey.h"
#include "Game.h"
#include "SeqToBin.h"
#include <iostream>

using namespace std;

int main()
{
    // Actor& actor = Actor::getInstance();
    // Command *cmd = new Command(CmdType::CMD_MOVE_UP);
    // cmd->doCommand(actor);
// Singleton<int> *s = new Singleton<int>();
    // auto s = Singleton<int>::getInstance();
    // Actor& a = Actor::getInstance();
    SeqToBin& s2b = SeqToBin::getInstance();
    Command* cmd = new Command(CMD_MOVE_DOWN);
    s2b.save2Bin(*cmd);
    // Actor *a = new Actor();
    // while (1) {
    //     cout<<int(read.scanKeyBoard())<<endl;
    // }
    return 0;
}