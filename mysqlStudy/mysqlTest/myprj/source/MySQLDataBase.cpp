#include "MySQLDataBase.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;

MySQLDataBase::MySQLDataBase()
{
	connection = mysql_init(NULL); // 初始化数据库的连接变量
	if(connection == NULL)
		{
			cout << "error:" << mysql_error(connection);
			exit(1);			// exit(1)表示发生错误后退出程序， exit(0)表示正常退出
		}
}

MySQLDataBase::~MySQLDataBase()
{
	if(connection != NULL)
		{
			mysql_close(connection); // 关闭数据库连接
		}
}

int MySQLDataBase::initDB(string host, string user, string password, string dbName)
{
	m_host = host;
	m_user = user;
	m_password = password;
	m_dbName = dbName;
	
    char value = 1;
    //mysql_options(connection,MYSQL_OPT_RECONNECT,(char*)&value);
	//mysql自动重连
	
	if (0 == mysql_options(connection,MYSQL_OPT_RECONNECT,(char*)&value)) {
		cout << "mysql_options() set autolink succeed" << endl;
		//return 0;
	}
	else{
		cout << "mysql_options() set autolink failed" << endl;
		return -1;
	}
	
	//设置支持中文
	if (0 == mysql_options(connection, MYSQL_SET_CHARSET_NAME, "gbk")) {
		cout << "mysql_options() set GBK succeed" << endl;
		//return 0;
	}
	else{
		cout << "mysql_options() set GBK failed" << endl;
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

int MySQLDataBase::exeSQL(string tableName)
{
    // mysql_query()执行成功返回0，失败返回非0值。与PHP中不一样
	string sql;
	sql = "select * from " + tableName + " ;";
	
    if(mysql_query(connection, sql.c_str()))
    {
		cout << "Query Error:" << mysql_error(connection);
        exit(1);
    }
    else
    {
        result = mysql_store_result(connection); // 获取结果集
        // mysql_use_result 和 mysql_fetch_row 进行配合使用
		//mysql_store_result（查询全部结果）和 mysql_num_rows（计算结果中的行数）
        for(int i=0; i < mysql_num_rows(result); ++i)
        {
            // 获取下一行
            row = mysql_fetch_row(result);
            if(row <= 0)
            {
                break;
            }
            // mysql_num_fields()返回结果集中的字段数
            for(int j=0; j < mysql_num_fields(result); ++j)
            {
				cout << row[j] << " ";
            }
			cout << endl;
        }
		cout << endl;
        // 释放结果集的内存
        mysql_free_result(result);
    }
    return 0;
}

int MySQLDataBase::insertSQL(string tableName,string insInfo)//map key/value
{
	insInfo="(id,name,class,grade):(1001,'gsc',3,100)";

	vector<string> res = mySplit(insInfo,":");
	string key = res[0];
	string value = res[1];
	
	cout<<"key:"<<key<<endl;
	cout<<"value:"<<value<<endl;
	
	string sql = "insert into "+ tableName + key + " values" + value +";";
	
	cout<<"sql:"<<sql<<endl;
	if(mysql_query(connection, sql.c_str()))
	{
		cout << "Query Error:" << mysql_error(connection);
		exit(1);
	}
	cout << endl;
	exeSQL(tableName);
	return 0;
}

int MySQLDataBase::deleteSQL(string tableName,string delInfo)
{
	string id;
	cout << "input the id you want to delete" << endl;
	cin >> id;
	string sql;
	sql = "delete from "+ tableName + " where id = " + id + ";";
	if(mysql_query(connection, sql.c_str()))
	{
		cout << "Query Error:" << mysql_error(connection);
		exit(1);
	}
	cout << endl;
	exeSQL(tableName);
	return 0;
	
}

int MySQLDataBase::updateSQL(string tableName,string updInfo)
{
	string id;
	string name;
	cout << "input the id you want to modify , and the modify content like name = 'Mary' " << endl;
	cin >> id >> name;
	string sql;
	sql = "update "+ tableName +" set " + name + "where id =" + id + ";";
	if(mysql_query(connection, sql.c_str()))
	{
		cout << "Query Error:" << mysql_error(connection);
		exit(1);
	}
	cout << endl;
	exeSQL(tableName);
	return 0;
	
}

vector<string> MySQLDataBase::mySplit(const string& str, string sp_string)  // split(), str 是要分割的string
{ 
    vector<string> vecString; 
    int sp_stringLen = sp_string.size(); 
    int lastPosition = 0; 
    int index = -1; 
	//如果要分割的串就是自己怎么写？
    while(-1 != (index=str.find(sp_string , lastPosition))) 
    { 
		if(str.substr(lastPosition, index - lastPosition) != string("")) //避免第一个字符串就是sp_string
		{
			vecString.push_back(str.substr(lastPosition, index - lastPosition));
		}
        lastPosition = index +sp_stringLen; 
    } 
    string lastStr = str.substr(lastPosition); 
    if ( !lastStr.empty() ) 
    { 
        vecString.push_back(lastStr); 
    } 
    return vecString; 
} 

void MySQLDataBase::Run()
{
		int i = 1;
	string tableName="studentInfo";
	while(i != 0)
		{
			int sel;
			printf("1.query    2.add     3.delete    4.modify    5.quit\n");
			//cout << "1.查询    2添加     3、删除    4、修改    5、退出" << endl;
			cin >> sel;
			switch(sel)
			{
			case 1:exeSQL(tableName);
				break;
			case 2:insertSQL(tableName,"");
				break;
			case 3:deleteSQL(tableName,"");
				break;
			case 4:updateSQL(tableName,"");
				break;
			case 5:i = 0;
				break;
			default:cout << "error" << endl;
				break;
			}
		}
}