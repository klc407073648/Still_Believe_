#ifndef __LOG_LOGGERMANAGER_H__
#define __LOG_LOGGERMANAGER_H__

#include<log4cpp/Category.hh>
#include<iostream>
#include <string>
#include <stdio.h>
#include <string.h>

namespace minilog{
//��־���ȼ�
enum Priority {
    ERROR,
    WARN,
    INFO,
    DEBUG
};

//�õ���ģʽ��װlog4cpp

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
			LoggerManager();  //����ģʽ�����캯��˽�л�

		 private:
			static LoggerManager *plog_;
			log4cpp::Category &category_ref_;
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


		//����һ������־����� �ļ��� ������ �к� �ĺ궨��
		#define suffix(msg)  std::string(msg).append(" ##")\
				.append(__FILE__).append(":").append(__func__)\
				.append(":").append(int2string(__LINE__))\
				.append("##").c_str()

		#define CREATE_LOG(logger) LoggerManager::setName(logger);
	
		//����ÿ��ʹ��ʱд getInstance���
		//ֻ��Ҫ���������ļ���д: #define _LOG4CPP_����������������ʹ��
		#ifdef _LOG4CPP_
				LoggerManager &mlog = LoggerManager::getInstance();
		#else
				extern LoggerManager &mlog;
		#endif
		
		//���̲��򻯺���������ʽ
		#define logError(msg) mlog.error(suffix(msg))
		#define logWarn(msg) mlog.warn(suffix(msg))
		#define logInfo(msg) mlog.info(suffix(msg))
		#define logDebug(msg) mlog.debug(suffix(msg))
}
#endif //__LOG_LOGGERMANAGER_H__
