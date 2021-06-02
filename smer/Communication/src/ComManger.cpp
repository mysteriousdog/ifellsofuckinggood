#ifdef SERVER_COMPARE
#include "ComManger.h"
#include <utility>
#include <vector>
using namespace std;

bool ComManger::addSessionTalker(int id, string&& name, int fd)
{
    vector<string> response;
    string idStr = to_string(id);
    string fdStr = to_string(fd);
    if (KGRedisClient::getInstance().ExecSadd(response, "userSession", move(idStr))) {
        cout<<"ComManger::addSessionTalker succ1: "<<endl;
        // for (auto it = response.begin(); it != response.end(); it++) {
        //     cout<<*it<<endl;
        // }
        vector<pair<string, string> > keyVals {{"name", name}, {"fd", fdStr}};

        if (!KGRedisClient::getInstance().ExecHMset(response, move(idStr),  keyVals)) {
            cout<<"ComManger::addSessionTalker error2: "<<endl;
            return false;
        }
        cout<<"ComManger::addSessionTalker succ2: "<<endl;
        return true;
    }
    cout<<"ComManger::addSessionTalker error1: "<<endl;
    return false;
}

bool ComManger::isTalkerOnline(int id)
{
    vector<string> response;
    if (KGRedisClient::getInstance().ExecScontain(response, "userSession", to_string(id))) {
        cout<<"ComManger::isTalkerOnline: "<<endl;
        for (auto it = response.begin(); it != response.end(); it++) {
            cout<<*it<<endl;
        }
        return true;
    }
    return false;
}

bool ComManger::removeSessionTalker(int id)
{
    vector<string> response;
    if (KGRedisClient::getInstance().ExecSremove(response, "userSession", to_string(id))) {
        cout<<"ComManger::removeSessionTalker1: "<<endl;
        if (KGRedisClient::getInstance().ExecDel(response, to_string(id))) {
            cout<<"ComManger::removeSessionTalker2: "<<endl;
            return true;
        }
        cout<<"ComManger::removeSessionTalker2 err: "<<endl;
        return false;
    }
    cout<<"ComManger::removeSessionTalker1 err: "<<endl;
    return false;
}

int ComManger::getTalkerFd(int id)
{
    vector<string> response;
    if (KGRedisClient::getInstance().ExecHget(response, to_string(id), "fd")) {
        int fd;
        try
        {
            fd = atoi(response[0].c_str());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            cout<<"getTalkerFd exception"<<endl;
            return -1;
        }
        return fd;
    }
    cout<<"getTalkerFd ExecHget"<<endl;
    return -1;
}

bool ComManger::getTalkerName(int id, string& res)
{
    vector<string> response;
    if (KGRedisClient::getInstance().ExecHget(response, to_string(id), "name")) {
        try
        {
            res = response[0];
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            cout<<"getTalkerName exception"<<endl;
            return false;
        }
        return true;
    }
    cout<<"getTalkerName ExecHget"<<endl;
    return false;
}

#endif