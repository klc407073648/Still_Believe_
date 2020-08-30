/**
 * @file MyfilesCgi.h
 * @brief 
 * @author klc
 * @date 2020-08-23
 * @copyright Copyright (c) 2020�� klc
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

#define HOST_NAME_LEN       (30)	//����ip��ַ����
#define TEMP_BUF_MAX_LEN    (512)	//��ʱ��������С
#define TIME_STRING_LEN     (25)    //ʱ�������
#define SQL_MAX_LEN         (512)   //sql�������ֵ


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
         * @brief  ��ȡ������json�ļ���Ϣ 
         *         ͬʱ�õ��ļ��ϴ��ߡ�У������
         *
         * @param [in]  myfiles_buf   ��������json����
         * @param [out] user      �ļ��ϴ���
         * @param [out] token     �ļ���У������
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getCountJsonInfo(const string& myfiles_buf, string& user, string& token);

        /**
         * @brief  ��ȡ�ļ��б��json�ļ���Ϣ 
         *         ͬʱ�õ��ļ��ϴ��ߡ�У������
         *
         * @param [in]  fileslist_buf   ��������json����
         * @param [out] user      �ļ��ϴ���
         * @param [out] token     �ļ���У������
         * @param [out] p_start   ��ʼλ��
         * @param [out] p_count   ����
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getFilesListJsonInfo(const string& fileslist_buf, string& user, string& token,int& start, int& count);
        
        /**
         * @brief  ��ȡ�û��ļ����� 
         *
         * @param [in] user      �ļ��ϴ���
         * @param [in] md5       �ļ���MD5��
         * @param [in] fileName    �ļ�����
         *
         */
        void getUserFilesCount(const string& user , int ret);
                /**
         * @brief  У���û����� 
         *
         * @param [in] user      �ļ��ϴ���
         * @param [in] token     �ļ���У������
         *
         * @returns
         *          0 succ, -1 fail
         */
        int verifyToken(const string& user, const string& token);

        /**
		* @brief ����md5��״̬
        * @param [in] num         ״̬��
        * @param [in] token_flag  token��־
		*/
        void returnLoginStatus(long num, int token_flag);
		void returnStatus(string status_num);

        /**
		* @brief ����md5��״̬
        * @param [in] cmd       ����
        * @param [in] token     �ļ�����
		*/
        int getUserFileList(const string& cmd, const string& user , int start, int count);

        /**
		* @brief md5��������߼�
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
