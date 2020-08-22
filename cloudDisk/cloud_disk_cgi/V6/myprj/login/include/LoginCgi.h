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
#include "Config.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace minilog;
using namespace std;

const string json_file = "./myprj/login/conf/conf.json";


class LoginCgi
{

public:
	LoginCgi();
        ~LoginCgi();
        int getLoginInfo(const string& buf, string& user, string& pwd);
        int checkUserAndPwd(string& user, string& pwd );
        int setToken(string& user, string& token);
        void returnLoginStatus(string status_num, string token);
        int loginExec();

private:
        DECLARE_STATIC_LOGGERMANAGER();
        map<string,string> m_ConfigMap;
        static Config* m_Config;
};

#endif //__STIBEL_LOGINCGI_H__
