#include"LoggerManager.h"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/Priority.hh>
#include<log4cpp/PropertyConfigurator.hh>

using namespace minilog;
using namespace std;

LoggerManager* LoggerManager::plog_ = NULL;

//获取log指针
LoggerManager& LoggerManager::getInstance() {
    if ( plog_ == NULL ) {
        plog_ = new LoggerManager();
    }
    return *plog_;
}


//销毁
void LoggerManager::destory() {
    if (plog_) {
        plog_->category_ref_.info("LoggerManager destroy");
        plog_->category_ref_.shutdown();
        delete plog_;
    }
}

std::string execShellPipe(std::string cmd)
{
	char res[1024]={0},*p;
	 
	FILE *fp = popen(cmd.c_str(),"r");
	
	if( fp != NULL)
	{
		fgets( res, sizeof(res), fp ); //遇到\n终止复制
		if((p = strchr(res,'\n')) != NULL)
			*p = '\0';
		//fread( res, sizeof(char), sizeof(res), fp );
		pclose(fp);
	}
	
	return res;
}


//构造函数
LoggerManager::LoggerManager(): 
    category_ref_(log4cpp::Category::getRoot()) {

    string mypath = execShellPipe("pwd") +"/myprj/log4cppTest/conf/log4cpp.conf";
    
    try
    {
        log4cpp::PropertyConfigurator::configure(mypath);
    }
    catch (log4cpp::ConfigureFailure& f)
    {
        std::cout << "Configure Problem: " << f.what() << std::endl;
        //return -1;
    }

    category_ref_.info("LoggerManager created!");
}


//设置优先级
void LoggerManager::setPriority(Priority priority) {
    switch (priority) {
        case (ERROR):
            category_ref_.setPriority(log4cpp::Priority::ERROR);
            break;
        case (WARN):
            category_ref_.setPriority(log4cpp::Priority::WARN);
            break;
        case (INFO):
            category_ref_.setPriority(log4cpp::Priority::INFO);
            break;
        case (DEBUG):
            category_ref_.setPriority(log4cpp::Priority::DEBUG);
            break;
        default:
            category_ref_.setPriority(log4cpp::Priority::DEBUG);
            break;    
    }
}


void LoggerManager::error(const char* msg) {
    category_ref_.error(msg);
}

void LoggerManager::warn(const char* msg) {
    category_ref_.warn(msg);
}

void LoggerManager::info(const char* msg) {
    category_ref_.info(msg);
}

void LoggerManager::debug(const char* msg) {
    category_ref_.debug(msg);
}
