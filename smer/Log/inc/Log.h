#ifndef _LOG_H_
#define _LOG_H_
#include "Singleton.h"
#include <boost/log/trivial.hpp>

#define LOG_DEBUG(str)\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::debug)) << str
#define LOG_INFO(str)\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::info)) << str
#define LOG_ERROR(str)\
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


#endif // _LOG_H_