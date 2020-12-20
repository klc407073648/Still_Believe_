/**
 * @file DealFileCgi.h
 * @brief ����ɾ���ļ����ļ�pv�ֶδ���CGI����
 * @author klc
 * @date 2020-08-26
 * @copyright Copyright (c) 2020�� klc
 */

#ifndef __STIBEL_DealFileCgi_H__
#define __STIBEL_DealFileCgi_H__

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

const string json_file = "./myprj/dealfile/conf/conf.json";

#define HOST_NAME_LEN       (30)	//����ip��ַ����
#define TEMP_BUF_MAX_LEN    (512)	//��ʱ��������С
#define TIME_STRING_LEN     (25)    //ʱ�������
#define SQL_MAX_LEN         (512)   //sql�������ֵ


struct dealFileInfo
{
    string cmd; 
    string user;    
    string token;
	string md5;
    string fileName;
};


class DealFileCgi
{

public:
		DealFileCgi();
        ~DealFileCgi();

        int getJsonInfo(const string& dealfile_buf, string& user, string& token ,string& md5, string& fileName);

        int shareFile(const string& user,const string& md5,const string& fileName);

        int delFile(const string& user,const string& md5,const string& fileName);

        int pvFile(const string& user,const string& md5,const string& fileName);

        int dealfileExec();
		
		void returnDealFileStatus(const string& statusNum);

        void initMySQL();

        void initMyRedis();

        int sqlExecResult(const string& sql_str);

private:
        DECLARE_STATIC_LOGGERMANAGER();
        void init();
        int removeFileFromFastDfs(const string&  fileId);
        int verifyToken(const string& user, const string& token);
		map<string,string> m_ConfigMap;
        static Config* m_Config;
        static MySQL::ptr mysql;
        static CMyRedis::ptr myRedis;
        dealFileInfo mFileInfo;
};

#endif //__STIBEL_DealFileCgi_H__
