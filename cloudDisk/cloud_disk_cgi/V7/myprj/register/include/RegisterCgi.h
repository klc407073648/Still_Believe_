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
#include "Config.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace minilog;
using namespace std;

const string json_file = "./myprj/register/conf/conf.json";

class RegisterCgi
{

public:
		RegisterCgi();
        ~RegisterCgi();
        int getRegInfo(const string& reg_buf, string& user, string& nick_name, string& pwd, string& tel, string& email);
        int userReg(string& reg_buf);
        void returnRegStatus(string status_num);
        int regExec();

private:
        DECLARE_STATIC_LOGGERMANAGER();
		map<string,string> m_ConfigMap;
        static Config* m_Config;
};

#endif //__STIBEL_REGISTERCGI_H__
