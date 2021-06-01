
#include "fun.h"
#include <iostream>
#include <boost/algorithm/string/split.hpp>

using namespace std;

void f1()
{
#ifdef SERVER_COMPARE
    cout<<"server f1"<<endl;
#endif

#ifdef CLIENT_COMPARE
    cout<<"client f1"<<endl;
#endif
}