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
        LOG_DEBUG("ComManger::addSessionTalker succ1: ");
        vector<pair<string, string> > keyVals {{"name", name}, {"fd", fdStr}};

        if (!KGRedisClient::getInstance().ExecHMset(response, move(idStr),  keyVals)) {
            LOG_ERR("ComManger::addSessionTalker error2: ");
            return false;
        }
        LOG_DEBUG("ComManger::addSessionTalker succ2: ");
        return true;
    }
    LOG_ERR("ComManger::addSessionTalker error1: ");
    return false;
}

bool ComManger::isTalkerOnline(int id)
{
    vector<string> response;
    if (KGRedisClient::getInstance().ExecScontain(response, "userSession", to_string(id))) {
        LOG_DEBUG("ComManger::isTalkerOnline: ");
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
        LOG_DEBUG("CComManger::removeSessionTalker1: ");
        if (KGRedisClient::getInstance().ExecDel(response, to_string(id))) {
            LOG_DEBUG("ComManger::removeSessionTalker2: ");
            return true;
        }
        LOG_ERR("ComManger::removeSessionTalker2 err: ");
        return false;
    }
    LOG_ERR("ComManger::removeSessionTalker1 err: ");
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
            LOG_ERR("getTalkerFd exception " + e.what());
            return -1;
        }
        return fd;
    }
    LOG_DEBUG("getTalkerFd ExecHget");
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
            LOG_ERR("getTalkerFd exception " + e.what());
            return false;
        }
        return true;
    }
    LOG_DEBUG("getTalkerName ExecHget");
    return false;
}

#endif