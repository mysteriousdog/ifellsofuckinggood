#ifndef _Z_TIME_H_
#define _Z_TIME_H_
#include "Singleton.h"
#include <iostream>
#include<boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class Ztime : public Singleton<Ztime>
{

public:
    string getCurTimeWithYmdhms() {
        string strPosixTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
        int pos = strPosixTime.find('T');
        strPosixTime.replace(pos,1,std::string("-"));    
        strPosixTime.replace(pos + 3,0,std::string(":"));    
        strPosixTime.replace(pos + 6,0,std::string(":"));    
        return strPosixTime;
    }

    string getCurTimeWithYmd() {
        string strPosixTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
        int pos = strPosixTime.find('T');
        return strPosixTime.substr(0, pos);
    }

    string getCurTimeWithHms() {
        string strPosixTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
        int pos = strPosixTime.find('T');
        strPosixTime.replace(pos + 3,0,std::string(":"));    
        strPosixTime.replace(pos + 6,0,std::string(":"));  
        return strPosixTime.substr(pos + 1);
    }

private:

friend class Singleton;

};

#endif