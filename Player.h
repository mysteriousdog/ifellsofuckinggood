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

    int id;
    string name;
    string password;

}playerData;

typedef struct FriendData {

    int id;
    FriendData(int id_): id(id_){}

}friendData;

class Player : public Singleton<Player> {

public:
    void init(string& name_, string& password_, int id_) {
        data.name = name_;
        data.password = password_;
        data.id = id_;
    }

    string getPlayerName() {
        return data.name;
    }

    string getPlayerPwd() {
        return data.password;
    }

    int getPlayerId() {
        return data.id;
    }

    string& getTalkerName() {
        return talkerName;
    }
    void setTalkerName(string&& name) {
        talkerName = name;
    }
    int getTalkerId() {
        return friends[talkerName]->id;
    }

    map<string, friendData*>& getAllFriends() {
        return friends;
    } 

    bool addFriend(string&& name, int id) {
        if (friends.size() >= MAX_FRIENDS_NUM) {
            return false;
        }
        if (friends.count(name) > 0) {
            friends[name]->id = id;
            isFriendsChanged = true;
            return true;
        }
        friends[name] = new friendData(id);
        isFriendsChanged = true;
        return true;
    }
    bool delFriend(string&& name) {
        if (friends.count(name) > 0) {
            if (friends[name] != nullptr) {
                delete(friends[name]);
            }
            friends[name] = nullptr;
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
    map<string, friendData*> friends;
    mutex data_mutex;
    condition_variable data_cond;
friend class Singleton;

};

#endif