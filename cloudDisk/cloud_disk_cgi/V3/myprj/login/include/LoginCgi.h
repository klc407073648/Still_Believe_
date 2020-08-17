/**
 * @file LoginCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_LOGINCGI_H__
#define __STIBEL_LOGINCGI_H__

#include<string>
#include "TEncode.h"
#include "LoggerManager.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "json/json.h"
#include "MySQL.h"
#include "CMyRedis.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace minilog;
using namespace std;


class LoginCgi
{

public:
		LoginCgi(map<string,string> cfg);
        int getLoginInfo(const string& buf, string& user, string& pwd);
        int checkUserAndPwd(string& user, string& pwd );
        int setToken(string& user, string& token);
        void returnLoginStatus(string status_num, string token);
        int loginExec();
		static map<string,string> getConfigMap()  { return m_ConfigMap;}

private:
        DECLARE_STATIC_LOGGERMANAGER();
		static map<string,string> m_ConfigMap;
};

#endif //__STIBEL_LOGINCGI_H__
