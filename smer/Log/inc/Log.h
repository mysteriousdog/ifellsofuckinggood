#ifndef _LOG_H_
#define _LOG_H_

#ifdef CLIENT_COMPARE

#include "Singleton.h"
#include <boost/log/trivial.hpp>

#define LOG_DEBUG(str)\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::debug)) << str
#define LOG_INFO(str)\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::info)) << str
#define LOG_ERR(str)\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::error)) << str
#define LOG_WARNING\

#define LOG_PATH "../Log"

class MyLog : public Singleton<MyLog>
{
public:
 
	MyLog();
 
	~MyLog(void);
 
	// 在使用之前必须先调用此函数
	static void Init(const std::string & dir);
 
	static void Log(const std::string& msg);
 
	static boost::log::sources::severity_logger<boost::log::trivial::severity_level > s_slg;
 
protected:
 
private:
friend class Singleton;
};

#endif // CLIENT_COMPARE



#ifdef SERVER_COMPARE

#include "Singleton.h"
#include "ZTime.h"
#include "FileIO.h"
#include "ConcQueue.h"

#include <iostream>
using namespace std;

#define LOG_PATH "../log/"

#define LOG_ERR(log)\
    do { \
        MyLog::getInstance().putErrLog(log); \
    } while(0)
#define LOG_DEBUG(log)\
    do { \
        MyLog::getInstance().putDebugLog(log); \
    } while(0)

#define LOG_INFO(log)\
    do { \
        MyLog::getInstance().putInfoLog(log); \
    } while(0)

#define LOG_WARNING(log)\
    do { \
        MyLog::getInstance().putWarningLog(log); \
    } while(0)

class MyLog : public Singleton<MyLog>
{

public:

    void run() {
        while (true) {
            std::cout<<"wait for log: "<<endl;
            if (logs.waitTillNotEmpty()) {
                auto log = logs.pop();
                auto file = LOG_PATH + Ztime::getInstance().getCurTimeWithYmd() + ".log";
                FileIo::getInstance().writeToFile(file, log);
            }
        }
    }

    void putLog(string& str) {
        std::cout<<"in logs: "<<str<<endl;
        logs.push(str);
    }
    void putErrLog(string str) {
        string level = " -ERROR- : ";
        string timeStamp = Ztime::getInstance().getCurTimeWithHms();
        string log = timeStamp + level + str + "\n";
        putLog(log);
    }

    void putDebugLog(string str) {
        string level = " -DEBUG- : ";
        string timeStamp = Ztime::getInstance().getCurTimeWithHms();
        string log = timeStamp + level + str + "\n";
        putLog(log);
    }

    void putInfoLog(string str) {
        string level = " -INFO- : ";
        string timeStamp = Ztime::getInstance().getCurTimeWithHms();
        string log = timeStamp + level + str + "\n";
        putLog(log);
    }

    void putWarningLog(string str) {
        string level = " -WARNING- : ";
        string timeStamp = Ztime::getInstance().getCurTimeWithHms();
        string log = timeStamp + level + str + "\n";
        putLog(log);
    }

private:
    MyLog(){}
    ConcQueue<string> logs;
friend class Singleton;

};

#endif


#endif // _LOG_H_