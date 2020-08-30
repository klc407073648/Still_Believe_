#include"LoggerManager.h"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/RollingFileAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PropertyConfigurator.hh>

using namespace minilog;
using namespace std;


LoggerManager::LoggerManager(const string name)
{
    m_InstanceName=name;	
	init();
}

LoggerManager::~LoggerManager() 
{
    m_Instance->info("LoggerManager destroy!");
    log4cpp::Category::shutdown();
}

void LoggerManager::init()
{
	m_Instance=&log4cpp::Category::getInstance(m_InstanceName);
	initAppender();
    //setFromConf();
}


//暂时不用，还未找到实例对应的appender初始化方法
void LoggerManager::setFromConf()
{
    try
    {
        log4cpp::PropertyConfigurator::configure(myLogPath);
    }
    catch (log4cpp::ConfigureFailure& f)
    {
        std::cout << "Configure Problem: " << f.what() << std::endl;
        //return -1;
    }
}
/*
RollingFileAppender(const std::string& name,  
                     const std::string& fileName,
                     size_t maxFileSize = 10*1024*1024,
                     unsigned int maxBackupIndex = 1,
                     bool append = true,
                     mode_t mode = 00644);
*/
void LoggerManager::initAppender()
{
	
    log4cpp::PatternLayout *pattern_one = new log4cpp::PatternLayout;
    pattern_one->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} - %p](%c):%m%n");

    log4cpp::PatternLayout *pattern_two = new log4cpp::PatternLayout;
    pattern_two->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} - %p](%c):%m%n");

    log4cpp::OstreamAppender *os_appender = new log4cpp::OstreamAppender("osAppender",&std::cout);
    os_appender->setLayout(pattern_one);
    
    log4cpp::RollingFileAppender *rolling_fileAppender = new log4cpp::RollingFileAppender("rollingfileAppender","./logs/log4cpp/mylog.log",5*1024,10);
    rolling_fileAppender->setLayout(pattern_two);

    setPriority(defPriority);

    m_Instance->addAppender(os_appender);
    m_Instance->addAppender(rolling_fileAppender);

	m_Instance->info("LoggerManager created, name is %s",m_InstanceName.c_str());
	
}


void LoggerManager::setPriority(Priority priority) {
    switch (priority) {
        case (ERROR):
            m_Instance->setPriority(log4cpp::Priority::ERROR);
            break;
        case (WARN):
            m_Instance->setPriority(log4cpp::Priority::WARN);
            break;
        case (INFO):
            m_Instance->setPriority(log4cpp::Priority::INFO);
            break;
        case (DEBUG):
            m_Instance->setPriority(log4cpp::Priority::DEBUG);
            break;
        default:
            m_Instance->setPriority(log4cpp::Priority::DEBUG);
            break;    
    }
}


string LoggerManager::getInstanceName()  const
{
    return m_InstanceName;
}

log4cpp::Category* LoggerManager::getCategory() const
{
	return m_Instance;
}
