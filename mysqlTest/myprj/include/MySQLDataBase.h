#ifndef _MYSQLDATABASE_H_
#define _MYSQLDATABASE_H_

#include <iostream>
#include <string>
#include "DataBaseInterface.h"
#include <mysql/mysql.h>
#include <string>
#include <vector>

using namespace std;

class MySQLDataBase //: public DataBaseInterface
{
public:
	MySQLDataBase();
	~MySQLDataBase();
	int initDB(string host, string user, string password, string dbName);
	
	int exeSQL(string tableName);
	int insertSQL(string tableName,string insInfo);
	int deleteSQL(string tableName,string delInfo);
	int updateSQL(string tableName,string updInfo);
	void Run();
    
	string getHost() const {return m_host;}
	string getUser() const {return m_user;}
	string getPassWord() const {return m_password;}
	string getDbName() const {return m_dbName;}
	
	vector<string> mySplit(const string& str, string sp_string); 
	
private:
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	string m_host;
	string m_user;
	string m_password;
	string m_dbName;
};

#endif