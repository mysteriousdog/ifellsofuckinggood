#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Singleton.h"
#include <iostream>
#include <map>
using namespace std;

extern const int NAME_MAX_LEN;
extern const int PASSWORD_MAX_LEN;
int const MAX_FRIENDS_NUM = 3000;

typedef struct PlayerData {

    string name;
    string password;

}playerData;

class Player : public Singleton<Player> {

public:
    void init(string& name_, string& password_) {
        data.name = name_;
        data.password = password_;
    }

    string& getTalkerName() {
        return talkerName;
    }
    void setTalkerName(string&& name) {
        talkerName = name;
    }

    map<string, bool>& getAllFriends() {
        return friends;
    } 

    bool addFriend(string&& name) {
        if (friends.size() >= MAX_FRIENDS_NUM) {
            return false;
        }
        friends[name] = true;
        isFriendsChanged = true;
        return true;
    }
    bool delFriend(string&& name) {
        if (friends.count(name) > 0) {
            friends[name] = false;
            isFriendsChanged = true;
            return true;
        }
        return false;
    }

private:
    Player(): isFriendsChanged(false){};
    playerData data;
    bool isFriendsChanged;
    string talkerName;
    map<string, bool> friends;
    mutex data_mutex;
    condition_variable data_cond;
friend class Singleton;

};

#endif