/**
 * @file MyfilesCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-23
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_MYFILESCGI_H__
#define __STIBEL_MYFILESCGI_H__

#include<string>
#include "TEncode.h"
#include "LoggerManager.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"
#include "json/json.h"
#include <mysql/mysql.h>
#include "CMyRedis.h"
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

const string json_file = "./myprj/myfiles/conf/conf.json";

#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SQL_MAX_LEN         (512)   //sql语句的最大值


struct myFilesInfo
{
    string cmd; 
    string user;    
    string token;
	int start;
	int count;
	
};


class MyFilesCgi
{

public:
		MyFilesCgi();
        ~MyFilesCgi();

        
		/**
         * @brief  获取计数的json文件信息 
         *         同时得到文件上传者、校验令牌
         *
         * @param [in]  myfiles_buf   待解析的json内容
         * @param [out] user      文件上传者
         * @param [out] token     文件的校验令牌
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getCountJsonInfo(const string& myfiles_buf, string& user, string& token);

        /**
         * @brief  获取文件列表的json文件信息 
         *         同时得到文件上传者、校验令牌
         *
         * @param [in]  fileslist_buf   待解析的json内容
         * @param [out] user      文件上传者
         * @param [out] token     文件的校验令牌
         * @param [out] p_start   开始位置
         * @param [out] p_count   数量
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getFilesListJsonInfo(const string& fileslist_buf, string& user, string& token,int& start, int& count);
        
        /**
         * @brief  获取用户文件数量 
         *
         * @param [in] user      文件上传者
         * @param [in] md5       文件的MD5码
         * @param [in] fileName    文件名称
         *
         */
        void getUserFilesCount(const string& user , int ret);
                /**
         * @brief  校验用户令牌 
         *
         * @param [in] user      文件上传者
         * @param [in] token     文件的校验令牌
         *
         * @returns
         *          0 succ, -1 fail
         */
        int verifyToken(const string& user, const string& token);

        /**
		* @brief 返回md5的状态
        * @param [in] num         状态码
        * @param [in] token_flag  token标志
		*/
        void returnLoginStatus(long num, int token_flag);
		void returnStatus(string status_num);

        /**
		* @brief 返回md5的状态
        * @param [in] cmd       命令
        * @param [in] token     文件令牌
		*/
        int getUserFileList(const string& cmd, const string& user , int start, int count);

        /**
		* @brief md5程序的主逻辑
		*/
        int myfilesExec();

private:
        DECLARE_STATIC_LOGGERMANAGER();
        void init();
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        myFilesInfo mFileInfo;
};

#endif //__STIBEL_MYFILESCGI_H__
