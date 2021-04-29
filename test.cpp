#include "Singleton.h"
#include "CommandDoer.h"
#include "MyEnum.h"
#include "Command.h"
#include "Actor.h"
#include "ReadKey.h"
#include "Game.h"
#include "SeqToBin.h"
#include "Client.h"
#include "SeqAbleObj.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// class A : public SeqAbleObj{
//     public:
//     virtual tansObj* getSeqData(){return nullptr;}
//     int getNum() {
//         return num;
//     }
//     int num;
// };

int main()
{
    // Actor& actor = Actor::getInstance();
    // Command *cmd = new Command(CmdType::CMD_MOVE_UP);
    // cmd->doCommand(actor);
// Singleton<int> *s = new Singleton<int>();
    // auto s = Singleton<int>::getInstance();
    // Actor& a = Actor::getInstance();
    SeqToBin& s2b = SeqToBin::getInstance();
    Command* cmd = new Command(CMD_MOVE_LEFT);
    // tansObj* obj = new tansObj(MSG_CMD, sizeof(*cmd));
    // memcpy(obj->msg, (void*)obj, obj->len);
    // cout<<cmd->getType()<<endl;
    // Command* cmd2 = (Command*)obj->msg;
    cout<<cmd->getType()<<endl;

    // Command a1(2);
    // // a1.num = 2;
    // cout<<a1.getType()<<endl;
    // void* msg = malloc(sizeof(Command));
    // memcpy(msg, &a1, sizeof(Command));
    // Command *a2 = (Command*)msg;
    // cout<<a2->getType()<<endl;
    s2b.save2Bin(*cmd);
    Client* client = new Client();
    cout<<"answer is "<<client->run()<<endl;
    // Actor *a = new Actor();
    // while (1) {
    //     cout<<int(read.scanKeyBoard())<<endl;
    // }
    return 0;
}