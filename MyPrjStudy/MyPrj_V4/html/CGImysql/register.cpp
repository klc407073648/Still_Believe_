#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <map>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <mysql/mysql.h>
#include "MutexLock.h"

#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"
#include "fcgi_stdio.h"


#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif


#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>


static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}


using namespace std;



int main(int argc, char *argv[])
{
	char **initialEnv = environ;
    int count = 0;

    while (FCGI_Accept() >= 0) 
	{
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;
		
		 printf("Content-type: text/html\r\n"
	    "\r\n"
	    "<title>FastCGI echo</title>"
	    "<h1>FastCGI echo</h1>\n");

	    printf("register begin<p>\n");

        if (contentLength != NULL) 
		{
            len = strtol(contentLength, NULL, 10);
			printf("len=%d<p>\n",len);
        }
        else 
		{
            len = 0;
        }

        if (len <= 0)
		{
			printf("No data from standard input.<p>\n");
        }
        else 
		{
            char *file_buf = NULL; //==========> 开辟存放文件的 内存 <===========
			file_buf = (char *)malloc(len);
		  
			if (file_buf == NULL)
			{
			   printf("malloc error! file size is to big!!!!\n");
			   return -1;
			}
		  
		    int ret = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			if(ret == 0)
			{
				printf("fread error\n");
				return -1;
			}
			printf("Standard input:<p>\n");
			printf("%s\<p>\n",file_buf);
           
		   //解析user password
			string mycontent= file_buf;
			
			int pos1=mycontent.find_first_of('=');
			int pos2=mycontent.find_first_of('&');
			int pos3=mycontent.find_last_of('=');
			
			std::string m_user = mycontent.substr(pos1+1,(pos2)-(pos1+1));
			std::string m_password = mycontent.substr(pos3+1);
			printf("m_user=%s <p>\n",m_user.c_str());
            printf("m_password=%s <p>\n",m_password.c_str());
		   
            //数据库判断
			 map<string, string> users;
			 std::string m_dbname = "yourdb";
   
		     MutexLock m_lock;
			 MYSQL *con = NULL;
			 con = mysql_init(con);
			
			if (con == NULL)
			{
				cout << "Error:" << mysql_error(con);
				exit(1);
			}
			
			con = mysql_real_connect(con, "localhost", "root", "klczxas789", m_dbname.c_str(), 3306, NULL, 0);

			if (con == NULL)
			{
				cout << "Error: " << mysql_error(con);
				exit(1);
			}
            
			//在user表中检索username，passwd数据，浏览器端输入
			if (mysql_query(con, "SELECT username,passwd FROM user"))
			{
				printf("INSERT error:%s\n", mysql_error(con));
				return -1;
			}
			//从表中检索完整的结果集
			MYSQL_RES *result = mysql_store_result(con);
			//返回结果集中的列数
			int num_fields = mysql_num_fields(result);
			//返回所有字段结构的数组
			MYSQL_FIELD *fields = mysql_fetch_fields(result);

			//从结果集中获取下一行，将对应的用户名和密码，存入map中
			while (MYSQL_ROW row = mysql_fetch_row(result))
			{
				string temp1(row[0]);
				string temp2(row[1]);
				users[temp1] = temp2;
			}

		  

			//flag=3如果是注册，先检测数据库中是否有重名的
			//没有重名的，进行增加数据
			string sql_insert("");
			sql_insert+="INSERT INTO user(username, passwd) VALUES('";
			sql_insert+=m_user;
			sql_insert+="', '";
			sql_insert+=m_password;
			sql_insert+="')";
 
			
				if (users.find(m_user) == users.end())
				{
					m_lock.lock();
					int res = mysql_query(con, sql_insert.c_str());
					m_lock.unlock();

					if (!res)
					{
						printf("register success<p>\n");
					}
					else
						printf("register fail<p>\n");//注册失败，显示注册错误界面
				}
				else
					printf("already register<p>\n");//已经注册，显示注册错误界面
			
		   
			//释放结果集使用的内存
			mysql_free_result(result);
        }
    } /* while */
	
    return 0;
}
