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
using namespace std;

const string myLogPath = "/home/MyPrj_V7/myprj/log4cppTest/conf/log4cpp.conf";

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
			 * @brief ���캯��
			 */
			LoggerManager(const string name="null"); 
			
			/**
			 * @brief ��������
			 * @param[in] newCategory    �����������
			 */
			~LoggerManager();
			
			/**
			 * @brief �������ȼ�
			 * @param[in] m         ��־����������
			 * @param[in] priority  ���ȼ�
			 */
			void setPriority(Priority priority);
			
			/**
			 * @brief дerror������־
			 * @param[in] m 	��־����������
			 * @param[in] msg   �����Ϣ
			 */
			void logError(const char* msg);
			
			/**
			 * @brief дwarn������־
			 * @param[in] m    ��־����������
			 * @param[in] msg  �����Ϣ
			 */
			void logWarn(const char* msg);
			
			/**
			 * @brief дinfo������־
			 * @param[in] m    ��־����������
			 * @param[in] msg  �����Ϣ
			 */
			void logInfo(const char* msg);
			
			/**
			 * @brief дdebug������־
			 * @param[in] m    ��־����������
			 * @param[in] msg  �����Ϣ
			 */
			void logDebug(const char* msg);
			
			/**
			 * @brief ���������������
			 * @param[in] newCategory    �����������
			 */
			void addLoggerCategory(); 
			
			/**
			 * @brief ��ʼ��ʵ������
			 * @param[in] name  ����
			 */
			void  getInstance(const string& name);
			
			/**
			 * @brief ��־�������ʵ������
			 */
			const string& getInstanceName() {return m_InstanceName;}
			log4cpp::Category* getCategory()  const
			{
				return m_Instance;
			}
		 private:
			/// ��־�������ʵ��
			log4cpp::Category* m_Instance;
			/// ��־�������ʵ������
			string 	m_InstanceName;
			/// ����־��
			log4cpp::Category* m_root; 
			///ʵ������
			int count;
		};

		//*****************************************************
		//ע�⣺
		//�ļ��� __FILE__ ,������ __func__ ���к�__LINE__ �Ǳ�����ʵ�ֵ�
		//����C++ͷ�ļ��ж���� 
		//ǰ����������string���ͣ���__LINE__�����Σ�������ҪתΪstring����
		//******************************************************

		//���������ļ��к� ->ת��Ϊstring����
		inline std::string int2string(int line) {
			std::ostringstream oss;
			oss << line;
			return oss.str();
		}
		
		//���������ļ��к� ->ת��Ϊstring����
		inline std::string flieNameSplit(std::string str) {
			std::string mStr=str;
			int begin=mStr.find_last_of('/');
			int end=mStr.find_last_of('.');
			return mStr.substr(begin+1,end-begin-1);
		}
		
		
		//����һ������־����� �ļ��� ������ �к� �ĺ궨��
		/* #define suffix(msg)  std::string(msg).append(" ##")\
				.append(__FILE__).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append("##").c_str() */
				
		#define suffix(msg)  std::string(msg).append("(")\
				.append(flieNameSplit(__FILE__)).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append(")").c_str()
		
		
		
		#define DECLARE_STATIC_LOGGERMANAGER()  static minilog::LoggerManager*  mlogger
		#define CREATE_STATIC_LOGGERMANAGER(_class)  minilog::LoggerManager* _class::mlogger =new minilog::LoggerManager(#_class)
		
		#define LOG_PACK(func,fmt,...)   func(fmt, ##__VA_ARGS__)

		#define LogDebug(fmt, ...)  LOG_PACK(mlogger->getCategory()->debug, fmt, ##__VA_ARGS__)
		#define LogInfo(fmt, ...)  LOG_PACK(mlogger->getCategory()->info, fmt, ##__VA_ARGS__)
		#define LogWarn(fmt, ...)  LOG_PACK(mlogger->getCategory()->warn, fmt, ##__VA_ARGS__)
		#define LogError(fmt, ...)  LOG_PACK(mlogger->getCategory()->error, fmt, ##__VA_ARGS__)		
}



#endif //__LOG_LOGGERMANAGER_H__
