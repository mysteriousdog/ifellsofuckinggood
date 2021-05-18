#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <vector>
#include "SeqAbleObj.h"
using namespace std;

#define INPUTSTR_SPLIT_FLAG "@"

typedef TransObj* (*COM_CMD_HANDLE_FUNC_PTR)(string&& input);
typedef struct CommunicationCmd {

    string comtype;
    COM_CMD_HANDLE_FUNC_PTR handler;

} CommunicationCmdTable;


class Util
{
public:
    Util(){}

    void splitStr(vector<string>& res, string&& source, string&& fg);
    TransObj* getMsgFromInput(string&& input);
private:
    

};

#endif // _UTIL_H_