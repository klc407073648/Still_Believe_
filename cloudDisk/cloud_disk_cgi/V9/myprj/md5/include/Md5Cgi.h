/**
 * @file md5Cgi.h
 * @brief 
 * @author klc
 * @date 2020-08-23
 * @copyright Copyright (c) 2020�� klc
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

#define HOST_NAME_LEN       (30)	//����ip��ַ����
#define TEMP_BUF_MAX_LEN    (512)	//��ʱ��������С
#define TIME_STRING_LEN     (25)    //ʱ�������
#define SQL_MAX_LEN         (512)   //sql�������ֵ


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
         * @brief  �����ϴ���post���� 
         *         ͬʱ�õ��ļ��ϴ��ߡ�У�����ơ�MD5�롢�ļ�����
         *
         * @param [in]  md5_buf   ��������json����
         * @param [out] user      �ļ��ϴ���
         * @param [out] token     �ļ���У������
         * @param [out] md5       �ļ���MD5��
         * @param [out] fileName    �ļ�����
         *
         * @returns
         *          0 succ, -1 fail
         */
        int getMd5Info(const string& md5_buf, string& user, string& token, string& md5, string& fileName);

        /**
         * @brief  MD5�봫���� 
         *
         * @param [in] user      �ļ��ϴ���
         * @param [in] md5       �ļ���MD5��
         * @param [in] fileName    �ļ�����
         *
         * @returns
         *      0,-1,-2,-3
         *     0    �봫�ɹ���  {"code":"006"}
         *    -1    ִ�г���
              -2    �ļ��Ѵ��ڣ�{"code":"005"}
              -3    �봫ʧ�ܣ�  {"code":"007"}

        */
        int dealMd5(const string& user , const string& md5, const string& fileName);

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
        * @param [in] status_num  ״̬��
		*/
        void returnMd5Status(const string& status_num);

        /**
		* @brief md5��������߼�
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
