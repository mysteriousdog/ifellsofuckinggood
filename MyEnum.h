#ifndef _MYENUM_H_
#define _MYENUM_H_



typedef enum CmdType{
    CMD_MOVE_UP,
    CMD_MOVE_DOWN,
    CMD_MOVE_LEFT,
    CMD_MOVE_RIGHT,
    CMD_BUTTON
} cmdType;
typedef unsigned int CMD_TYPE_UINT32_ENUM;


typedef enum MsgType{
    MSG_CMD = 0,
    MSG_BUTTON
} msgType;
typedef unsigned int MSG_TYPE_UINT32_ENUM;


#endif