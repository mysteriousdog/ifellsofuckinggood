#include "ReadKey.h"
#include "Command.h"
#include <stdio.h>
#include <iostream>
#include<unistd.h>
#include<termios.h>

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