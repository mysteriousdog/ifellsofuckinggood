#ifndef _MYENUM_H_
#define _MYENUM_H_

#include <stdint.h>

typedef enum CmdType{
    CMD_MOVE_UP,
    CMD_MOVE_DOWN,
    CMD_MOVE_LEFT,
    CMD_MOVE_RIGHT,
    CMD_BUTTON
} cmdType;
typedef uint32_t CMD_TYPE_UINT32_ENUM;

#endif