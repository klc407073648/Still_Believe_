/**
 * @file LoginCgi.h
 * @brief 登陆CGI程序
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
#define SQL_MAX_LEN         (512)   //sql语句长度

struct loginFileInfo
{
    string user;     
    string pwd;    
    string token;
};

class LoginCgi
{

public:
	LoginCgi();
        ~LoginCgi();
        int getLoginInfo(const string& login_buf,string& user,string& pwd);
        int checkUserAndPwd(const string& user,const string& pwd );
        int setToken(const string& user, string& token);
        void returnLoginStatus(const string& statusNum, const string& token);
        int loginExec();
        MySQL::ptr getMySQL();

private:
        DECLARE_STATIC_LOGGERMANAGER();
        map<string,string> m_ConfigMap;
        static Config* m_Config;
        static MySQL::ptr mysql;
        loginFileInfo mFileInfo;
};

#endif //__STIBEL_LOGINCGI_H__
