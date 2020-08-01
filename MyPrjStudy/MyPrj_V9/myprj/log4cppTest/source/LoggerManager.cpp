#include"LoggerManager.h"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PropertyConfigurator.hh>

using namespace minilog;
using namespace std;


LoggerManager::LoggerManager(string name)
{
    //m_root=&log4cpp::Category::getRoot();
	count=0;	
	getInstance(name);
}

LoggerManager::~LoggerManager() {
    //info(m_root,"LoggerManager destroy!");
    m_Instance->info("LoggerManager destroy!");
    log4cpp::Category::shutdown();
}

void LoggerManager::getInstance(const string& name){
    if ( count == 0 )
	{
		count=1;
		m_InstanceName=name;
        //setFromConf();
		m_Instance=&log4cpp::Category::getInstance(m_InstanceName);
		addLoggerCategory();
    }
}

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

void LoggerManager::addLoggerCategory()
{
	
    log4cpp::PatternLayout *pattern_one = new log4cpp::PatternLayout;
    pattern_one->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} - %p](%c):%m%n");

    log4cpp::PatternLayout *pattern_two = new log4cpp::PatternLayout;
    pattern_two->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S} - %p](%c):%m%n");

    log4cpp::OstreamAppender *os_appender = new log4cpp::OstreamAppender("osAppender",&std::cout);
    os_appender->setLayout(pattern_one);

    
    log4cpp::FileAppender *file_appender = new log4cpp::FileAppender("fileAppender","./logs/log4cpp/mylog.log");
    file_appender->setLayout(pattern_two);

    setPriority(Priority::DEBUG);
    m_Instance->addAppender(os_appender);
	//setPriority(Priority::DEBUG);
    m_Instance->addAppender(file_appender);

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
