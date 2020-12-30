/**
 * @file ShareFilesCgi.h
 * @brief 分享、删除文件、文件pv字段处理CGI程序
 * @author klc
 * @date 2020-08-26
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_ShareFilesCgi_H__
#define __STIBEL_ShareFilesCgi_H__

#include<string>
#include "TEncode.h"
#include "LoggerManager.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "json/json.h"
#include "MySQL.h"
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

const string json_file = "./myprj/sharefiles/conf/conf.json";

#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SQL_MAX_LEN         (512)   //sql语句的最大值


struct shareFilesInfo
{
    string cmd; 
    int start;
    int count;
};


class ShareFilesCgi
{

public:
		ShareFilesCgi();
        ~ShareFilesCgi();

        int getFilesListJsonInfo(const string& sharefiles_buf , int& start, int& count);
        int getRankFileList(const int& start, const int& count);
        int getShareFileList(const int& start, const int& count);
        void getShareFilesCount();
        void returnShareFilesStatus(const int& ret_type);
        int sharefilesExec();

        void initMySQL();
        void initMyRedis();
        int sqlExecResult(const string& sql_str);

private:
        DECLARE_STATIC_LOGGERMANAGER();
        void init();
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        static MySQL::ptr mysql;
        static CMyRedis::ptr myRedis;
        shareFilesInfo mFileInfo;
        Json::Value m_result;
};

#endif //__STIBEL_ShareFilesCgi_H__
