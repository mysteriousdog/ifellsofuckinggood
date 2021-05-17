#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <vector>
#include "SeqAbleObj.h"
using namespace std;

#define INPUTSTR_SPLIT_FLAG "@"

class Util
{
public:
    Util(){}

    void splitStr(vector<string>& res, string& source, string&& fg);
    TransObj* getMsgFromInput(string& input);
private:
    

};

#endif // _UTIL_H_