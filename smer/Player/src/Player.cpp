#include "Player.h"
#include "SeqAbleObj.h"
#include "Log.h"
#include <fstream>


const int NAME_MAX_LEN = 24;
const int PASSWORD_MAX_LEN = MAX_TRANS_MSG_LEN - NAME_MAX_LEN;


bool Player::savePlayerData()
{
    ofstream fout(PLAYER_DATA_FILE, ios::binary);   
    if (fout.is_open()) {       
        boost::archive::binary_oarchive oa(fout);
        oa << *(this);
        fout.close();
        return true;
    }
    else {
        LOG_ERR("save player data err!");
        return false;
    } 
}

bool Player::loadPlayerData()
{
    ifstream fin(PLAYER_DATA_FILE, ios::in|ios::binary);    
    fin.seekg(0, ios::end);
    streampos fp = fin.tellg(); 
    if (fin.is_open()&&fp) {    
        //然后要把文件指针放到文件头部
        fin.seekg(0);
        boost::archive::binary_iarchive ia(fin);
        ia >> (*this);

        // //event对象信息填写
        // event.eid = data["eid"];
        // event.eventname = data["eventname"];
        // event.starttime = data["starttime"];
        // event.genderforevent = data["genderforevent"];
        // event.is_canceled = data["is_canceled"];
        // event.location = data["location"];
        // event.statusforevent = data["statusforevent"];
        // event.players.push_back(data["player"]);

        // events.insert(pair<string, Events>(event.eid, event));      
        fin.close();
        return true;
    } 
    else {
        LOG_ERR("read player data err!");
        return false;
    } 
}