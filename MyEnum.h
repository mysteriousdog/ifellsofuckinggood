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
    MSG_TALK,
    MSG_ACK,
    MSG_REG,
    MSG_REG_ACCEPT,
    MSG_REG_REFUSE,
    MSG_LOGIN,
    MSG_LOGIN_ACCEPT,
    MSG_LOGIN_REFUSE,
    MSG_LOGOUT,
    MSG_ASK_FOR_FRIEND,
    MSG_BUTTON
} msgType;
typedef unsigned int MSG_TYPE_UINT32_ENUM;


#endif