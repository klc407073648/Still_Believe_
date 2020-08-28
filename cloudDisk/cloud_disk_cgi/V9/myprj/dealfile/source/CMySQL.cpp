#include "CMySQL.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;

CMySQL::CMySQL()
{
	connection = mysql_init(NULL); // ��ʼ�����ݿ�����ӱ���
	if(connection == NULL)
		{
			cout << "error:" << mysql_error(connection);
			exit(1);			// exit(1)��ʾ����������˳����� exit(0)��ʾ�����˳�
		}
}

CMySQL::~CMySQL()
{
	if(connection != NULL)
		{
			mysql_close(connection); // �ر����ݿ�����
		}
}

int CMySQL::initDB(const string& host, const string& user, const string& password, const string& dbName)
{
	m_host = host;
	m_user = user;
	m_password = password;
	m_dbName = dbName;

    char value = 1;
    //mysql_options(connection,MYSQL_OPT_RECONNECT,(char*)&value);
	//mysql�Զ�����
	
	if (0 == mysql_options(connection,MYSQL_OPT_RECONNECT,(char*)&value)) {
		//cout << "mysql_options() set autolink succeed" << endl;
		//return 0;
	}
	else{
		//cout << "mysql_options() set autolink failed" << endl;
		return -1;
	}
	
	//����֧������
	if (0 == mysql_options(connection, MYSQL_SET_CHARSET_NAME, "gbk")) {
		//cout << "mysql_options() set GBK succeed" << endl;
		//return 0;
	}
	else{
		//cout << "mysql_options() set GBK failed" << endl;
		return -1;
	}

	connection = mysql_real_connect(connection, m_host.c_str(), m_user.c_str(), m_password.c_str(), m_dbName.c_str(), 0, NULL, 0); // �������ݿ�����
	if(connection == NULL)
	{
		cout << "error:" << mysql_error(connection);
		exit(1);			// exit(1)��ʾ����������˳����� exit(0)��ʾ�����˳�
	}
	return 0;
}

void CMySQL::print_error(const char *title)
{
    fprintf(stderr,"%s:\nError %u (%s)\n", title, mysql_errno(connection), mysql_error(connection));
}

int CMySQL::query(char *sql_cmd)
{
	return mysql_query(connection, sql_cmd);
}

int CMySQL::exeSQL(char *sql_cmd, char *buf)
{
	int ret = 0;
	ulong line = 0;
    if(mysql_query(connection, sql_cmd))
    {
		print_error("mysql_query error!\n");
        ret = -1;
        goto END;
    }
    
    result = mysql_store_result(connection); // ��ȡ�����

		if (result == NULL)
		{
			print_error("mysql_store_result error!\n");
			ret = -1;
			goto END;
		}

    	//mysql_num_rows������mysql_store_result���صĽ���ṹ���������ؽṹ���е�����
    	line = mysql_num_rows(result);

        if (line == 0)
		{
			ret = 1; //1û�м�¼��
			goto END;
		}
		else if(line > 0 && buf == NULL) //���bufΪNULL�����豣��������ֻ���ж���û�д˼�¼
		{
			ret = 2; //2�м�¼������û�б���
			goto END;
		}

		// mysql_fetch_row�ӽ���ṹ����ȡһ�У��������ŵ�һ���нṹ�С������������������ʱ����NULL.
		if (( row = mysql_fetch_row(result) ) != NULL)
		{
			if (row[0] != NULL)
			{
				strcpy(buf, row[0]);
			}
		}
END:
    if(result != NULL)
    {
        //������ж����ݵĲ����󣬵���mysql_free_result���ƺ���
        mysql_free_result(result);
    }
    
    return 0;
}








