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
using namespace std;

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
			 * @brief 构造函数
			 */
			LoggerManager(const string name="null"); 
			
			/**
			 * @brief 析构函数
			 * @param[in] newCategory    新增对象类别
			 */
			~LoggerManager();
			
			/**
			 * @brief 设置优先级
			 * @param[in] m         日志输出对象类别
			 * @param[in] priority  优先级
			 */
			void setPriority(Priority priority);
			
			/**
			 * @brief 写error级别日志
			 * @param[in] m 	日志输出对象类别
			 * @param[in] msg   输出消息
			 */
			void logError(const char* msg);
			
			/**
			 * @brief 写warn级别日志
			 * @param[in] m    日志输出对象类别
			 * @param[in] msg  输出消息
			 */
			void logWarn(const char* msg);
			
			/**
			 * @brief 写info级别日志
			 * @param[in] m    日志输出对象类别
			 * @param[in] msg  输出消息
			 */
			void logInfo(const char* msg);
			
			/**
			 * @brief 写debug级别日志
			 * @param[in] m    日志输出对象类别
			 * @param[in] msg  输出消息
			 */
			void logDebug(const char* msg);
			
			/**
			 * @brief 新增输出对象类型
			 * @param[in] newCategory    新增对象类别
			 */
			void addLoggerCategory(); 
			
			/**
			 * @brief 初始化实例对象
			 * @param[in] name  名称
			 */
			void  getInstance(const string& name);
			
			/**
			 * @brief 日志输出对象实例名称
			 */
			const string& getInstanceName() {return m_InstanceName;}
			
		 private:
			/// 日志输出对象实例
			log4cpp::Category* m_Instance;
			/// 日志输出对象实例名称
			string 	m_InstanceName;
			/// 主日志器
			log4cpp::Category* m_root; 
			///实例个数
			int count;
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
		
		//整数类型文件行号 ->转换为string类型
		inline std::string flieNameSplit(std::string str) {
			std::string mStr=str;
			int begin=mStr.find_last_of('/');
			int end=mStr.find_last_of('.');
			return mStr.substr(begin+1,end-begin-1);
		}
		
		
		//定义一个在日志后添加 文件名 函数名 行号 的宏定义
		/* #define suffix(msg)  std::string(msg).append(" ##")\
				.append(__FILE__).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append("##").c_str() */
				
		#define suffix(msg)  std::string(msg).append("(")\
				.append(flieNameSplit(__FILE__)).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append(")").c_str()
		
		
		
		#define DECLARE_STATIC_LOGGERMANAGER()  static LoggerManager*  mlogger
		#define CREATE_STATIC_LOGGERMANAGER(_class)  LoggerManager* _class::mlogger =new LoggerManager(#_class)
		
}



#endif //__LOG_LOGGERMANAGER_H__
