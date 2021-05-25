#include "ReadKey.h"
#include "Command.h"
#include "SeqToBin.h"
#include <stdio.h>
#include <iostream>
#include<unistd.h>
#include<termios.h>
#include <string.h>
#include "IOManger.h"
using namespace std;

bool ReadKey::isSystemCmd(CMD_TYPE_UINT32_ENUM type)
{
    if (type >= CMD_ENTER && type <= CMD_SYSTEM) {
        return true;
    }
    return false;
}

bool ReadKey::isActorCmd(CMD_TYPE_UINT32_ENUM type)
{
    if (type >= CMD_MOVE_UP && type <= CMD_JUMP) {
        return true;
    }
    return false;
}

bool ReadKey::isTimeToReadKey()
{
    return !IOManger::getInstance().isTalking() && !IOManger::getInstance().isOutputing();
}

CMD_TYPE_UINT32_ENUM ReadKey::scanKeyBoard()
{
    if (!isTimeToReadKey()) {
        return CMD_BUTTON;
    }
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    // cout<<new_settings.c_lflag<<endl;
    // cout<<(new_settings.c_cc[VTIME] == 0)<<endl;
    // cout<<(new_settings.c_cc[VMIN] == 1)<<endl;
    new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= ~ECHO;// 关闭回显
    new_settings.c_cc[VTIME] = 0;
    // tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    
    in = getchar();
    
    tcsetattr(0,TCSANOW,&stored_settings);
    std::cout<<"get "<<in<<std::endl;
    if (cmd.find(in) == cmd.end()) {
        return CMD_BUTTON;
    }
    Command* comd = new Command(cmd[in]);
    SeqToBin& sb = SeqToBin::getInstance();
    tansObj* obj = new tansObj(MSG_CMD, sizeof(Command));
    memcpy(obj->msg, (void*)comd, obj->len);
    if (isActorCmd(cmd[in])) {
        sb.getBuff().push(obj);
        cmdQue.push(comd);
    } else if (isSystemCmd(cmd[in])) {
        std::cout<<"get system"<<cmd[in]<<std::endl;
        sysCmdQue.push(comd);
    } else {
        delete(obj);
    }
    return cmd[in];
}

ReadKey::ReadKey()
{
    cmd[119] = CMD_MOVE_UP;
    cmd[97] = CMD_MOVE_LEFT;
    cmd[115] = CMD_MOVE_DOWN;
    cmd[100] = CMD_MOVE_RIGHT;
    cmd[10] = CMD_ENTER;
    cmd[98] = CMD_BACKPACK;
    cmd[109] = CMD_MAP;
    cmd[27] = CMD_SYSTEM;
    cmd[106] = CMD_ATTACK;
    cmd[32] = CMD_JUMP;
}

Command* ReadKey::getCmd()
{
    if (cmdQue.empty()) {
        return nullptr;
    }
    Command* cmd = cmdQue.front();
    cmdQue.pop();
    return cmd;
}

Command* ReadKey::getSysCmd()
{
    if (sysCmdQue.empty()) {
        return nullptr;
    }
    Command* cmd = sysCmdQue.front();
    sysCmdQue.pop();
    return cmd;
}