#ifndef __STIBEL_CMySQL_H__
#define __STIBEL_CMySQL_H__

#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

using namespace std;

class CMySQL //: public DataBaseInterface
{
public:
	typedef std::shared_ptr<CMySQL> ptr;	
	CMySQL();
	~CMySQL();
	int initDB(const string& host, const string& user, const string& password, const string& dbName);
	
	void print_error(const char *title);

	int exeSQL(char *sql_cmd, char *buf);
	int query(char *sql_cmd);
  
	string getHost() const {return m_host;}
	string getUser() const {return m_user;}
	string getPassWord() const {return m_password;}
	string getDbName() const {return m_dbName;}
	MYSQL* getConn() const {return connection;}
	string getErrStr()  { return mysql_error(connection);}
	
	
private:
	//static CMySQL::ptr m_MySQL;
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	string m_host;
	string m_user;
	string m_password;
	string m_dbName;
};

#endif //__STIBEL_CMySQL_H__