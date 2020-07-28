#ifndef __LOG_LOGGERMANAGER_H__
#define __LOG_LOGGERMANAGER_H__

#include<log4cpp/Category.hh>
#include<iostream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace minilog{
//日志优先级
enum Priority {
    ERROR,
    WARN,
    INFO,
    DEBUG
};

//用单例模式封装log4cpp

		class LoggerManager {
		 public: 
			static LoggerManager& getInstance();
			static void destory();

			void setPriority(Priority priority);
			void error(const char* msg);
			void warn(const char* msg);
			void info(const char* msg);
			void debug(const char* msg);

		 private:
			LoggerManager();  //单例模式：构造函数私有化

		 private:
			static LoggerManager *plog_;
			log4cpp::Category &category_ref_;
		};

		//*****************************************************
		//注意：
		//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
		//并非C++头文件中定义的 
		//前两个变量是string类型，且__LINE__是整形，所以需要转为string类型
		//******************************************************

		//整数类型文件行号 ->转换为string类型
		inline std::string int2string(int line) {
			std::ostringstream oss;
			oss << line;
			return oss.str();
		}


		//定义一个在日志后添加 文件名 函数名 行号 的宏定义
		#define suffix(msg)  std::string(msg).append(" ##")\
				.append(__FILE__).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append("##").c_str()

		#define CREATE_LOG(logger) LoggerManager::setName(logger);
	
		//不用每次使用时写 getInstance语句
		//只需要在主函数文件中写: #define _LOG4CPP_即可在整个程序中使用
		#ifdef _LOG4CPP_
				LoggerManager &mlog = LoggerManager::getInstance();
		#else
				extern LoggerManager &mlog;
		#endif
		
		//缩短并简化函数调用形式
		#define logError(msg) mlog.error(suffix(msg))
		#define logWarn(msg) mlog.warn(suffix(msg))
		#define logInfo(msg) mlog.info(suffix(msg))
		#define logDebug(msg) mlog.debug(suffix(msg))
}
#endif //__LOG_LOGGERMANAGER_H__
