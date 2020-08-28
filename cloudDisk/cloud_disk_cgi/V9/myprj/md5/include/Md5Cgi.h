/**
 * @file md5Cgi.h
 * @brief 
 * @author klc
 * @date 2020-08-23
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_MD5CGI_H__
#define __STIBEL_MD5CGI_H__

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

const string json_file = "./myprj/md5/conf/conf.json";

#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SQL_MAX_LEN         (512)   //sql语句的最大值


struct md5FileInfo
{
    string fileName; 
    string user;   
    string md5;   
    string token;
};


class Md5Cgi
{

public:
		Md5Cgi();
        ~Md5Cgi();

        
		/**
         * @brief  解析上传的post数据 
         *         同时得到文件上传者、校验令牌、MD5码、文件名称
         *
         * @param [in]  md5_buf   待解析的json内容
         * @param [out] user      文件上传者
         * @param [out] token     文件的校验令牌
         * @param [out] md5       文件的MD5码
         * @param [out] fileName    文件名称
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getMd5Info(const string& md5_buf, string& user, string& token, string& md5, string& fileName);

        /**
         * @brief  MD5秒传处理 
         *
         * @param [in] user      文件上传者
         * @param [in] md5       文件的MD5码
         * @param [in] fileName    文件名称
         *
         * @returns
         *      0,-1,-2,-3
         *     0    秒传成功：  {"code":"006"}
         *    -1    执行出错
              -2    文件已存在：{"code":"005"}
              -3    秒传失败：  {"code":"007"}

        */
        int dealMd5(const string& user , const string& md5, const string& fileName);

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
        * @param [in] status_num  状态码
		*/
        void returnMd5Status(const string& status_num);

        /**
		* @brief md5程序的主逻辑
		*/
        int md5Exec();

private:
        DECLARE_STATIC_LOGGERMANAGER();
        void init();
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        //static MySQL::ptr mysql;
        //static CMyRedis::ptr myRedis;
        md5FileInfo mFileInfo;
};

#endif //__STIBEL_MD5CGI_H__
