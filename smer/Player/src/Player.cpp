
#include "Player.h"
#include "SeqAbleObj.h"
#include "Log.h"
#include <fstream>


const int NAME_MAX_LEN = 24;
const int PASSWORD_MAX_LEN = MAX_TRANS_MSG_LEN - NAME_MAX_LEN;

#ifdef CLIENT_COMPARE
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
#endif

#ifdef CLIENT_COMPARE
bool Player::loadPlayerData()
{
    ifstream fin(PLAYER_DATA_FILE, ios::in|ios::binary);    
    fin.seekg(0, ios::end);
    streampos fp = fin.tellg(); 
    if (fin.is_open()&&fp) {    
        fin.seekg(0);
        boost::archive::binary_iarchive ia(fin);
        ia >> (*this);
        fin.close();
        return true;
    } 
    else {
        LOG_ERR("read player data err!");
        return false;
    } 
}
#endif