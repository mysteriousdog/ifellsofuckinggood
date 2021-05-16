#ifndef _MYENUM_H_
#define _MYENUM_H_



typedef enum CmdType{
    CMD_MOVE_UP,
    CMD_MOVE_DOWN,
    CMD_MOVE_LEFT,
    CMD_MOVE_RIGHT,
    CMD_ATTACK,
    CMD_JUMP,
    CMD_ENTER,
    CMD_BACKPACK,
    CMD_MAP,
    CMD_SYSTEM,
    CMD_BUTTON
} cmdType;
typedef unsigned int CMD_TYPE_UINT32_ENUM;


typedef enum MsgType{
    MSG_CMD = 0,
    MSG_USER_SEND = 1,
    MSG_USER_RECV = 2,
    MSG_TALK = 3,
    MSG_ACK = 4,
    MSG_REG = 5,
    MSG_REG_ACCEPT = 6,
    MSG_REG_REFUSE = 7,
    MSG_LOGIN = 8,
    MSG_LOGIN_ACCEPT = 9,
    MSG_LOGIN_REFUSE = 10,
    MSG_BUTTON
} msgType;
typedef unsigned int MSG_TYPE_UINT32_ENUM;


#endif