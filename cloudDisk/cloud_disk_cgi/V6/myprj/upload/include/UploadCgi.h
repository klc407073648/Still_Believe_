/**
 * @file UploadCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-21
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_UploadCgi_H__
#define __STIBEL_UploadCgi_H__

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

const string json_file = "./myprj/upload/conf/conf.json";

#define HOST_NAME_LEN       (30)	//主机ip地址长度
#define TEMP_BUF_MAX_LEN    (512)	//临时缓冲区大小
#define TIME_STRING_LEN     (25)    //时间戳长度
#define SQL_MAX_LEN         (512)   //sql语句的最大值


struct fileInfo
{
    string fileName; 
    string user;   
    string md5;   
    long size; 
    string fileId;    
    string fdfsFileUrl; 
};


class UploadCgi
{

public:
		UploadCgi();
        ~UploadCgi();

        
		/**
         * @brief  解析上传的post数据 保存到本地临时路径
         *         同时得到文件上传者、文件名称、文件大小
         *
         * @param [in]  len       post数据的长度
         * @param [out] user      文件上传者
         * @param [out] fileName  文件的文件名
         * @param [out] md5       文件的MD5码
         * @param [out] p_size    文件大小
         *
         * @returns
         *          0 succ, -1 fail
         */
        int recSaveFile(long len, string& user, string& fileName, string& md5, long& p_size);

        /**
         * @brief  将一个本地文件上传到 后台分布式文件系统中
         *
         * @param [in]  fileName  本地文件的路径
         * @param [out] fileId    得到上传之后的文件ID路径
         *
         * @returns
         *      0 succ, -1 fail
         */
        int uploadToFastDfs(const string& fileName, string& fileId);

        /**
         * @brief  封装文件存储在分布式系统中的 完整 url
         *
         * @param [in]  fileId          文件分布式id路径
         * @param [out] fdfsFileUrl     文件的完整url地址
         *
         * @returns
         *      0 succ, -1 fail
         */
        int makeFileUrl(const string& fileId, string& fdfsFileUrl);

        /**
         * @brief  存储文件信息到Mysql数据库
         *
         * @param [in] user         文件上传者
         * @param [in] fileName     文件的文件名
         * @param [in] md5          文件的MD5码
         * @param [in] size         文件大小
         * @param [in] fileId       文件ID
         * @param [in] fdfsFileUrl  文件Url信息
         *
         * @returns
         *          0 succ, -1 fail
         */
        int storeFileInfoToMysql(const string& user,const string& fileName,const string& md5, const long& size,const string& fileId, const string& fdfsFileUrl);

        /**
		* @brief 返回Upload的状态
		*/
        void returnUploadStatus(string status_num);

        /**
		* @brief 上传程序的主逻辑
		*/
        int uploadExec();

private:
        DECLARE_STATIC_LOGGERMANAGER();
        string parseContent(string src, string target);
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        fileInfo mFileInfo;
};

#endif //__STIBEL_UploadCgi_H__
