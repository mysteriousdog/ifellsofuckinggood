#include "ReadKey.h"
#include "Command.h"
#include "SeqToBin.h"
#include <stdio.h>
#include <iostream>
#include<unistd.h>
#include<termios.h>
#include <string.h>

CMD_TYPE_UINT32_ENUM ReadKey::scanKeyBoard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= ~ECHO;// 关闭回显
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    
    in = getchar();
    
    tcsetattr(0,TCSANOW,&stored_settings);
    Command* comd = new Command(cmd[in]);
    SeqToBin& sb = SeqToBin::getInstance();
    tansObj* obj = new tansObj(MSG_CMD, sizeof(Command));
    memcpy(obj->msg, (void*)comd, obj->len);
    sb.getBuff().push(obj);
    cmdQue.push(comd);
    return cmd[in];
}

ReadKey::ReadKey()
{
    cmd[119] = CMD_MOVE_UP;
    cmd[97] = CMD_MOVE_LEFT;
    cmd[115] = CMD_MOVE_DOWN;
    cmd[100] = CMD_MOVE_RIGHT;
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