#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include "Actor.h"
#include "ReadKey.h"
#include "Game.h"
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
    Game& game = Game::getInstance();
    game.updateWordPerSec();
    // Actor *a = new Actor();
    // while (1) {
    //     cout<<int(read.scanKeyBoard())<<endl;
    // }
    return 0;
}