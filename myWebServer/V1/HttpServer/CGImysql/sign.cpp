#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <map>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "MutexLock.h"
using namespace std;



int main(int argc, char *argv[])
{
    map<string, string> users;
   std::string m_user = "";
   std::string m_password = "";
   std::string m_dbname = "";
   std::string flag = "";
   
   MutexLock m_lock;
   int opt;
    const char *str = "u:p:n:f:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'u':
        {
            m_user = optarg;
            break;
        }
        case 'p':
        {
            m_password = optarg;
            break;
        }
        case 'n':
        {
            m_dbname = optarg;
            break;
        }
		case 'f':
        {
            flag = optarg;
            break;
        }
        default:
            break;
        }
    }
   
   

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

        if (flag == "3")
        {
            if (users.find(m_user) == users.end())
            {
                m_lock.lock();
                int res = mysql_query(con, sql_insert.c_str());
                m_lock.unlock();

                if (!res)
                    printf("1\n");//注册成功，显示登陆界面
                else
                    printf("0\n");//注册失败，显示注册错误界面
            }
            else
                printf("0\n");//已经注册，显示注册错误界面
        }
       //如果是登录，直接判断
        //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
        else if (flag == "2")
        {
            if (users.find(m_user) != users.end() && users[m_user] == m_password)
                printf("1\n");
            else
                printf("0\n");
        }
        else
            printf("0\n");
        //释放结果集使用的内存
        mysql_free_result(result);
    
 
	return 0;
}
