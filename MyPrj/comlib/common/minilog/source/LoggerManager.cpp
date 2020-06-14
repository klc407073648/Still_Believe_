#include"LoggerManager.h"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/Priority.hh>

using namespace minilog;
using namespace std;

//int LoggerManager::count=0;
//LoggerManager* LoggerManager::m_loggerManger = NULL;//root 节点

LoggerManager::LoggerManager(string name)
{
    m_root=&log4cpp::Category::getRoot();
	count=0;
	//if(m_loggerManger==NULL)
	//{
	//	m_loggerManger=this;
	//}
	
	getInstance(name);

	//log4cpp::PatternLayout *root_pattern = new log4cpp::PatternLayout;
    //root_pattern->setConversionPattern("%d{%Y-%m-%d,%H:%M:%S}: %p %c:%m%n");


    //log4cpp::OstreamAppender *os_appender = new log4cpp::OstreamAppender("osAppender",&std::cout);
    //os_appender->setLayout(root_pattern);

	//m_root->addAppender(os_appender);
   
	//info(m_root,"LoggerManager created!");
}

LoggerManager::~LoggerManager() {
    //info(m_root,"LoggerManager destroy!");   
    m_root->shutdown();
}

void LoggerManager::getInstance(const string& name){
    if ( count == 0 )
	{
		count=1;
		m_InstanceName=name;
		m_Instance=&log4cpp::Category::getRoot().getInstance(m_InstanceName);
		addLoggerCategory();
    }
}



void LoggerManager::addLoggerCategory()
     {
	
    log4cpp::PatternLayout *pattern_one = new log4cpp::PatternLayout;
    pattern_one->setConversionPattern("%d{%Y-%m-%d,%H:%M:%S}: %p %c:%m%n");

	//pattern_one->setConversionPattern("%d{%Y-%m-%d,%H:%M:%S}: %p %c %x:%m%n");
	//%c 输出日志信息所属的类目，通常就是所在类的全名
    log4cpp::PatternLayout *pattern_two = new log4cpp::PatternLayout;
    pattern_two->setConversionPattern("%d{%Y-%m-%d,%H:%M:%S}: %p %c:%m%n");

    //获取屏幕输出
    log4cpp::OstreamAppender *os_appender = new log4cpp::OstreamAppender("osAppender",&std::cout);
    os_appender->setLayout(pattern_one);

    //获取文件日志输出 （ 日志文件名:mylog.txt )
    log4cpp::FileAppender *file_appender = new log4cpp::FileAppender("fileAppender","../logs/mylog.txt");
    file_appender->setLayout(pattern_two);

    setPriority(Priority::DEBUG);
    m_Instance->addAppender(os_appender);
    m_Instance->addAppender(file_appender);

	//info("newCategory created success!");
    //info(name,"LoggerManager created!");
	
}


//设置优先级
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


void LoggerManager::logError(const char* msg) {
    m_Instance->error(msg);
}

void LoggerManager::logWarn(const char* msg) {
    m_Instance->warn(msg);
}

void LoggerManager::logInfo(const char* msg) {
    m_Instance->info(msg);
}

void LoggerManager::logDebug(const char* msg) {
    m_Instance->debug(msg);
}
