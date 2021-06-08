#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Singleton.h"
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <boost/archive/binary_iarchive.hpp> //二进制序列化
#include <boost/archive/binary_oarchive.hpp> //二进制序列化
#include <boost/serialization/vector.hpp> //序列化STL容器要导入
#include <boost/serialization/map.hpp> //序列化STL容器要导入
using namespace std;

#define PLAYER_DATA_FILE "../data/player.data"
extern const int NAME_MAX_LEN;
extern const int PASSWORD_MAX_LEN;
int const MAX_FRIENDS_NUM = 3000;
typedef struct PlayerData {

    int id;
    string name;
    string password;
    PlayerData(){}
    PlayerData(int id_, string& name_, string& password_):
    id(id_),
    name(name_),
    password(password_)
    {}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar& id;
        ar& name;
        ar& password;
    }

}playerData;

typedef struct FriendData {

    int id;
    FriendData(){};
    FriendData(int id_): id(id_){}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar& id;
    }

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

    void setPlayerId(int id_) {
        data.id = id_;
    }

    void setPlayerName(const char* name_) {
        data.name = name_;
    }

    void setPlayerPasswd(const char* passwd_) {
        data.password = passwd_;
    }

    void setPlayerLogin(int id_, const char* name_, const char* passwd_) {
        setLoginStatus(true);
        setPlayerId(id_);
        setPlayerName(name_);
        setPlayerPasswd(passwd_);
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
#ifdef CLIENT_COMPARE
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
        return savePlayerData();
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
        return savePlayerData();
    }
#endif
    void setLoginStatus(bool isLogined) {
        logined = isLogined;
    }
    bool isLogined() {
        return logined;
    }
#ifdef CLIENT_COMPARE
    bool savePlayerData();
    bool loadPlayerData();
#endif
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar& data.id;
        ar& data.name;
        ar& data.password;
        ar& friends;
    }
    
    
private:
    bool logined;
    Player(): isFriendsChanged(false), logined(false){};
    playerData data;
    bool isFriendsChanged;
    string talkerName;
    map<string, friendData*> friends;
    mutex data_mutex;
    condition_variable data_cond;
friend class Singleton;
friend class boost::serialization::access;
};



#endif
