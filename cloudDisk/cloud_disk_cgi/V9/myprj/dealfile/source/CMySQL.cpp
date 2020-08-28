#include "CMySQL.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;

CMySQL::CMySQL()
{
	connection = mysql_init(NULL); // 初始化数据库的连接变量
	if(connection == NULL)
		{
			cout << "error:" << mysql_error(connection);
			exit(1);			// exit(1)表示发生错误后退出程序， exit(0)表示正常退出
		}
}

CMySQL::~CMySQL()
{
	if(connection != NULL)
		{
			mysql_close(connection); // 关闭数据库连接
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
	//mysql自动重连
	
	if (0 == mysql_options(connection,MYSQL_OPT_RECONNECT,(char*)&value)) {
		//cout << "mysql_options() set autolink succeed" << endl;
		//return 0;
	}
	else{
		//cout << "mysql_options() set autolink failed" << endl;
		return -1;
	}
	
	//设置支持中文
	if (0 == mysql_options(connection, MYSQL_SET_CHARSET_NAME, "gbk")) {
		//cout << "mysql_options() set GBK succeed" << endl;
		//return 0;
	}
	else{
		//cout << "mysql_options() set GBK failed" << endl;
		return -1;
	}

	connection = mysql_real_connect(connection, m_host.c_str(), m_user.c_str(), m_password.c_str(), m_dbName.c_str(), 0, NULL, 0); // 建立数据库连接
	if(connection == NULL)
	{
		cout << "error:" << mysql_error(connection);
		exit(1);			// exit(1)表示发生错误后退出程序， exit(0)表示正常退出
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
    
    result = mysql_store_result(connection); // 获取结果集

		if (result == NULL)
		{
			print_error("mysql_store_result error!\n");
			ret = -1;
			goto END;
		}

    	//mysql_num_rows接受由mysql_store_result返回的结果结构集，并返回结构集中的行数
    	line = mysql_num_rows(result);

        if (line == 0)
		{
			ret = 1; //1没有记录集
			goto END;
		}
		else if(line > 0 && buf == NULL) //如果buf为NULL，无需保存结果集，只做判断有没有此记录
		{
			ret = 2; //2有记录集但是没有保存
			goto END;
		}

		// mysql_fetch_row从结果结构中提取一行，并把它放到一个行结构中。当数据用完或发生错误时返回NULL.
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
        //完成所有对数据的操作后，调用mysql_free_result来善后处理
        mysql_free_result(result);
    }
    
    return 0;
}








