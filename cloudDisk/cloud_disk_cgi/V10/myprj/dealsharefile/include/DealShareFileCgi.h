/**
 * @file DealShareFileCgi.h
 * @brief 共享文件pv字段处理、取消分享、转存文件
 * @author klc
 * @date 2020-08-26
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_DealShareFileCgi_H__
#define __STIBEL_DealShareFileCgi_H__

#include<string>
#include "TEncode.h"
#include "LoggerManager.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "json/json.h"
#include "CMySQL.h"
#include "CMyRedis.h"
#include "Util.h"
#include <sys/time.h>
#include "Config.h"
#include <sys/types.h>
#include <sys/wait.h>
#include<stdio.h>

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace minilog;
using namespace std;

const string json_file = "./myprj/dealsharefile/conf/conf.json";

#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SQL_MAX_LEN         (512)   //sql语句的最大值


struct dealShareFileInfo
{
    string cmd; 
    string user;
    string md5;
    string fileName;
};


class DealShareFileCgi
{

public:
		DealShareFileCgi();
        ~DealShareFileCgi();

        int getJsonInfo(const string& dealsharefile_buf , string& user ,string& md5 , string& fileName);
        int pvFile(const string& md5 ,const string& fileName);
        int cancelShareFile(const string& user,const string& md5 ,const string& fileName);
        int saveFile(const string& user,const string& md5 ,const string& fileName);
        void returnStatus(string status_num);
        int dealsharefileExec();
        
private:
        DECLARE_STATIC_LOGGERMANAGER();
        void init();
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        dealShareFileInfo mFileInfo;
};

#endif //__STIBEL_DealShareFileCgi_H__
