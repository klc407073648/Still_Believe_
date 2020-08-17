/**
 * @file LoggerManager.h
 * @brief ��־ģ���װ
 * @author klc
 * @date 2020-03-01
 * @copyright Copyright (c) 2020�� klc
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
	 * @brief ��־���ȼ�
	 */
	enum Priority {
		ERROR,
		WARN,
		INFO,
		DEBUG
	};
	
	/**
	 * @brief ��־������
	 */
	class LoggerManager {
		 public: 
			/**
			 * @brief Ĭ�Ϲ��캯��
			 */
			LoggerManager(); 

			/**
			 * @brief �����ƵĹ��캯��
			 * @param[in] name ��־������
			 */
			LoggerManager(const string& name); 
			
			/**
			 * @brief ��������
			 */
			~LoggerManager();
	
			/**
			 * @brief ����root�����ȼ���һ��ֱ���޸�root���ȼ�Ӱ��m_Instance���ȼ�
			 * @param[in] priority  ���ȼ�
			 */
			void setRootPriority(Priority& priority);

			/**
			 * @brief ���������ļ�����Appender������
			 * @param[in] confName  �����ļ�����
			 */
			void setFromConf(const string& confName);
			
			/**
			 * @brief �ֶ���ʼ��Appender�����ԣ���ʱ����
			 * @param[in] newCategory    �����������
			 */
			void initAppender(); 
			
			/**
			 * @brief ��ʼ������
			 */
			void  init();
			
			/**
			 * @brief ��־�������ʵ������
			 */
			string getInstanceName() const;

			/**
			 * @brief ��־�������ʵ��
			 */
			log4cpp::Category* getCategory() const;
			
		 private:
			/// ��־�������ʵ��
			log4cpp::Category* m_Instance;
			/// ��־�������ʵ������
			string 	m_InstanceName;
			/// ����־��
			log4cpp::Category* m_root; 
		
		};
	
}



#endif //__LOG_LOGGERMANAGER_H__
