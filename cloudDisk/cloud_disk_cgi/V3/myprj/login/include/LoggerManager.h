/**
 * @file LoggerManager.h
 * @brief 日志模块封装
 * @author klc
 * @date 2020-03-01
 * @copyright Copyright (c) 2020年 klc
 */


#ifndef __LOG_LOGGERMANAGER_H__
#define __LOG_LOGGERMANAGER_H__

#include<log4cpp/Category.hh>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"common.h"

using namespace std;

#define DECLARE_STATIC_LOGGERMANAGER()  static minilog::LoggerManager*  mlogger
#define CREATE_STATIC_LOGGERMANAGER(_class)  minilog::LoggerManager* _class::mlogger =new minilog::LoggerManager(#_class)
		
#define LOG_PACK(func,fmt,...)   {func(fmt, ##__VA_ARGS__);}

#define LogDebug(fmt, ...)  LOG_PACK(mlogger->getCategory()->debug, fmt, ##__VA_ARGS__)
#define LogInfo(fmt, ...)  LOG_PACK(mlogger->getCategory()->info, fmt, ##__VA_ARGS__)
#define LogWarn(fmt, ...)  LOG_PACK(mlogger->getCategory()->warn, fmt, ##__VA_ARGS__)
#define LogError(fmt, ...)  LOG_PACK(mlogger->getCategory()->error, fmt, ##__VA_ARGS__)	

namespace minilog{
	/**
	 * @brief 日志优先级
	 */
	enum Priority {
		ERROR,
		WARN,
		INFO,
		DEBUG
	};
	
	/**
	 * @brief 日志管理类
	 */
	class LoggerManager {
		 public: 
			/**
			 * @brief 默认构造函数
			 */
			LoggerManager(); 

			/**
			 * @brief 带名称的构造函数
			 * @param[in] name 日志器名称
			 */
			LoggerManager(const string& name); 
			
			/**
			 * @brief 析构函数
			 */
			~LoggerManager();
	
			/**
			 * @brief 设置root的优先级，一般直接修改root优先级影响m_Instance优先级
			 * @param[in] priority  优先级
			 */
			void setRootPriority(Priority& priority);

			/**
			 * @brief 利用配置文件设置Appender的属性
			 * @param[in] confName  配置文件名称
			 */
			void setFromConf(const string& confName);
			
			/**
			 * @brief 手动初始化Appender的属性，暂时废弃
			 * @param[in] newCategory    新增对象类别
			 */
			void initAppender(); 
			
			/**
			 * @brief 初始化函数
			 */
			void  init();
			
			/**
			 * @brief 日志输出对象实例名称
			 */
			string getInstanceName() const;

			/**
			 * @brief 日志输出对象实例
			 */
			log4cpp::Category* getCategory() const;
			
		 private:
			/// 日志输出对象实例
			log4cpp::Category* m_Instance;
			/// 日志输出对象实例名称
			string 	m_InstanceName;
			/// 主日志器
			log4cpp::Category* m_root; 
		
		};
	
}



#endif //__LOG_LOGGERMANAGER_H__
