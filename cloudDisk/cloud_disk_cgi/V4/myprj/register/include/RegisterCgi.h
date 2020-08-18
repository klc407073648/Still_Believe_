/**
 * @file RegisterCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_REGISTERCGI_H__
#define __STIBEL_REGISTERCGI_H__

#include<string>
#include "TEncode.h"
#include "LoggerManager.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "json/json.h"
#include "MySQL.h"
#include "CMyRedis.h"
#include <sys/time.h>

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace minilog;
using namespace std;


class RegisterCgi
{

public:
		RegisterCgi(map<string,string> cfg);
        int getRegInfo(const string& reg_buf, string& user, string& nick_name, string& pwd, string& tel, string& email);
        int userReg(string& reg_buf);
        void returnRegStatus(string status_num);
        int regExec();
		static map<string,string> getConfigMap()  { return m_ConfigMap;}

private:
        DECLARE_STATIC_LOGGERMANAGER();
		static map<string,string> m_ConfigMap;
};

#endif //__STIBEL_REGISTERCGI_H__
