/**
 * @file UploadCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-21
 * @copyright Copyright (c) 2020�� klc
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

#define HOST_NAME_LEN       (30)	//����ip��ַ����
#define TEMP_BUF_MAX_LEN    (512)	//��ʱ��������С
#define TIME_STRING_LEN     (25)    //ʱ�������
#define SQL_MAX_LEN         (512)   //sql�������ֵ


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
         * @brief  �����ϴ���post���� ���浽������ʱ·��
         *         ͬʱ�õ��ļ��ϴ��ߡ��ļ����ơ��ļ���С
         *
         * @param [in]  len       post���ݵĳ���
         * @param [out] user      �ļ��ϴ���
         * @param [out] fileName  �ļ����ļ���
         * @param [out] md5       �ļ���MD5��
         * @param [out] p_size    �ļ���С
         *
         * @returns
         *          0 succ, -1 fail
         */
        int recSaveFile(long len, string& user, string& fileName, string& md5, long& p_size);

        /**
         * @brief  ��һ�������ļ��ϴ��� ��̨�ֲ�ʽ�ļ�ϵͳ��
         *
         * @param [in]  fileName  �����ļ���·��
         * @param [out] fileId    �õ��ϴ�֮����ļ�ID·��
         *
         * @returns
         *      0 succ, -1 fail
         */
        int uploadToFastDfs(const string& fileName, string& fileId);

        /**
         * @brief  ��װ�ļ��洢�ڷֲ�ʽϵͳ�е� ���� url
         *
         * @param [in]  fileId          �ļ��ֲ�ʽid·��
         * @param [out] fdfsFileUrl     �ļ�������url��ַ
         *
         * @returns
         *      0 succ, -1 fail
         */
        int makeFileUrl(const string& fileId, string& fdfsFileUrl);

        /**
         * @brief  �洢�ļ���Ϣ��Mysql���ݿ�
         *
         * @param [in] user         �ļ��ϴ���
         * @param [in] fileName     �ļ����ļ���
         * @param [in] md5          �ļ���MD5��
         * @param [in] size         �ļ���С
         * @param [in] fileId       �ļ�ID
         * @param [in] fdfsFileUrl  �ļ�Url��Ϣ
         *
         * @returns
         *          0 succ, -1 fail
         */
        int storeFileInfoToMysql(const string& user,const string& fileName,const string& md5, const long& size,const string& fileId, const string& fdfsFileUrl);

        /**
		* @brief ����Upload��״̬
		*/
        void returnUploadStatus(string status_num);

        /**
		* @brief �ϴ���������߼�
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
