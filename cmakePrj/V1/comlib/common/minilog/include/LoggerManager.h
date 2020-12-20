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
		
		
		
		#define DECLARE_STATIC_LOGGERMANAGER()  static LoggerManager*  mlogger
		#define CREATE_STATIC_LOGGERMANAGER(_class)  LoggerManager* _class::mlogger =new LoggerManager(#_class)
		
}



#endif //__LOG_LOGGERMANAGER_H__
