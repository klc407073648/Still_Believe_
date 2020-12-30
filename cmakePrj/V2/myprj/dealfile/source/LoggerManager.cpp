#include"LoggerManager.h"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/RollingFileAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PropertyConfigurator.hh>

using namespace minilog;
using namespace std;

LoggerManager::LoggerManager()
{
   m_root=&log4cpp::Category::getRoot();
   m_Instance=NULL;
   m_InstanceName="root";
   init();
}

LoggerManager::LoggerManager(const string& name)
{
    m_InstanceName=name;
    m_root=&log4cpp::Category::getRoot();
	m_Instance=&log4cpp::Category::getInstance(m_InstanceName);	
	init();
}

LoggerManager::~LoggerManager() 
{
    m_root->info("LoggerManager destroy, name is %s",m_InstanceName.c_str());
    m_root->shutdown();
}

void LoggerManager::init()
{
	//initAppender();
    //const string myConfLogPath = "./myprj/login/conf/log4cpp.conf";
    //const string myLogPath = "./conf/log4cpp.conf";
    setFromConf("./myprj/dealfile/conf/log4cpp.conf");
	
	if (m_Instance == NULL)
	{
		m_root->info("LoggerManager created, name is %s",m_InstanceName.c_str());
	}
	else
	{
		m_Instance->info("LoggerManager created, name is %s",m_InstanceName.c_str());
	}
}


void LoggerManager::setFromConf(const string& confName)
{
    try
    {
        log4cpp::PropertyConfigurator::configure(confName);
    }
    catch (log4cpp::ConfigureFailure& f) 
    {
        std::cout << "Configure Problem: " << f.what() <<std::endl;
		
        //作保护，如果读取的配置文件不存在，使用默认文件
		try
		{
			std::cout << "use default configure file: log4cpp.conf"<<std::endl;
			log4cpp::PropertyConfigurator::configure("./conf/log4cpp.conf");
		}
		catch (log4cpp::ConfigureFailure& f) 
		{
			std::cout << "Configure Problem(default): " << f.what() <<std::endl;
		}
    }
}

//该函数废弃
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

    //setPriority(Priority::DEBUG);

    m_Instance->addAppender(os_appender);
    m_Instance->addAppender(rolling_fileAppender);

	m_Instance->info("LoggerManager created, name is %s",m_InstanceName.c_str());
	
}

/*
使用方法：
#include "Config.h"

CREATE_STATIC_LOGGERMANAGER(Config);

Config::Config()
{
	Priority mPriority= Priority::DEBUG;
	mlogger->setRootPriority(mPriority);
    readConfFromJson();
	LogDebug("Config()");
}
*/
void LoggerManager::setRootPriority(Priority& priority) {
    switch (priority) {
        case (ERROR):
            m_root->setPriority(log4cpp::Priority::ERROR);
            break;
        case (WARN):
            m_root->setPriority(log4cpp::Priority::WARN);
            break;
        case (INFO):
            m_root->setPriority(log4cpp::Priority::INFO);
            break;
        case (DEBUG):
            m_root->setPriority(log4cpp::Priority::DEBUG);
            break;
        default:
            m_root->setPriority(log4cpp::Priority::DEBUG);
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
