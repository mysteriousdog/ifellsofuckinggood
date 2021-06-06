#ifndef _LOG_H_
#define _LOG_H_

#ifdef CLIENT_COMPARE

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

#endif // CLIENT_COMPARE



#ifdef SERVER_COMPARE

#include "log4cplus/loglevel.h"  
#include "log4cplus/ndc.h"   
#include "log4cplus/logger.h"  
#include "log4cplus/configurator.h"  
#include "iomanip"  
#include "log4cplus/fileappender.h"  
#include "log4cplus/layout.h"  
  
#include "const.h"  
#include "common.h"  
#include "Main_config.h"  
  
using namespace log4cplus;  
using namespace log4cplus::helpers;  
  
//日志封装  
#define TRACE(p) LOG4CPLUS_TRACE(Log::_logger, p)  
#define DEBUG(p) LOG4CPLUS_DEBUG(Log::_logger, p)  
#define NOTICE(p) LOG4CPLUS_INFO(Log::_logger, p)  
#define WARNING(p) LOG4CPLUS_WARN(Log::_logger, p)  
#define FATAL(p) LOG4CPLUS_ERROR(Log::_logger, p)  
  
// 日志控制类，全局共用一个日志  
class Log  
{  
public:  
    // 打开日志  
    bool open_log();  
  
    // 获得日志实例  
    static Log& instance();  
      
    static Logger _logger;  
  
private:  
    Log();  
  
    virtual ~Log();  
  
    //log文件路径及名称  
    char _log_path[PATH_SIZE];  
    char _log_name[PATH_SIZE];  
};  

#endif


#endif // _LOG_H_