#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Singleton.h"
#include <iostream>
using namespace std;

extern const int NAME_MAX_LEN;
extern const int PASSWORD_MAX_LEN;

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

private:
    Player(){};
    playerData data; 
friend class Singleton;

};

#endif