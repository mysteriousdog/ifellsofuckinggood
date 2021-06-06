#ifdef CLIENT_COMPARE
#include "Log.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/detail/format.hpp>

using namespace std;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
 
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt)
boost::log::sources::severity_logger<boost::log::trivial::severity_level > MyLog::s_slg;
 
MyLog::MyLog()
{
}
 
MyLog::~MyLog(void)
{
 
}
 
void MyLog::Init(const std::string& dir)
{
	if (boost::filesystem::exists(dir) == false)
	{
		boost::filesystem::create_directories(dir);
        cout<<"cretae dir "<<dir<<endl;
	}
 
	auto pSink = logging::add_file_log
		(
		keywords::open_mode = std::ios::app, // 采用追加模式
		keywords::file_name = dir + "/%Y%m%d.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		//keywords::format = "[%TimeStamp% %ThreadID%]: %Message%"
		keywords::format =
		(
		expr::stream
		<< "[" << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		<< " " << expr::attr< boost::log::aux::thread::id >("ThreadID")
		<< " " << logging::trivial::severity
		<< "] " << expr::smessage
		)
		);
	// 如果不写这个 它不会实时的把日志写下去, 而是等待缓冲区满了,或者程序正常退出时写下,这样做的好处是减少IO操作.提高效率,  不过我这里不需要它. 因为我的程序可能会异常退出.
	pSink->locked_backend()->auto_flush(true);//使日志实时更新
	//pSink->imbue(std::locale("zh_CN.UTF-8")); // 本地化 
	logging::add_common_attributes();
}

void MyLog::Log(const std::string& msg)
{
	src::logger lg;
	BOOST_LOG(lg) << msg;
}

#endif // client


#ifdef SERVER_COMPARE

#include "Log.h"

Logger Log::_logger = log4cplus::Logger::getInstance("main_log");  
  
Log::Log()  
{  
    snprintf(_log_path, sizeof(_log_path), "%s", "../log");  
    snprintf(_log_name, sizeof(_log_name), "%s/%s.%s", _log_path, execname, "log");  
}  
  
Log::~Log()  
{  
}  
  
Log& Log::instance()  
{  
    static Log log;  
    return log;  
}  
  
bool Log::open_log()  
{  
      
    int Log_level = Main_config::instance().get_config().Read("LOG_LEVEL", 0);    
  
    /* step 1: Instantiate an appender object */  
    SharedAppenderPtr _append(new FileAppender(_log_name));  
    _append->setName("file log test");  
  
    /* step 2: Instantiate a layout object */  
    std::string pattern = "[%p] [%d{%m/%d/%y %H:%M:%S}] [%t] - %m %n";  
    std::auto_ptr<Layout> _layout(new PatternLayout(pattern));  
  
    /* step 3: Attach the layout object to the appender */  
    _append->setLayout(_layout);  
  
    /* step 4: Instantiate a logger object */  
  
    /* step 5: Attach the appender object to the logger  */  
    Log::_logger.addAppender(_append);  
  
    /* step 6: Set a priority for the logger  */  
    Log::_logger.setLogLevel(Log_level);  
  
    return true;  
}  

#endif //server