#ifndef _DATABASEINTERFACE_H_
#define _DATABASEINTERFACE_H_
/******************************************************************* 
简要描述:数据库基类
创建日期: 2020.4.25
作者: klc
版本：v1.0
修改日期: 
******************************************************************/
#include <string>

using namespace std;

class DataBaseInterface
{
public:
	virtual int exeSQL(string tableName) = 0;
	virtual int insertSQL(string tableName,string insInfo) = 0;
	virtual int deleteSQL(string tableName,string delInfo) = 0;
	virtual int updateSQL(string tableName,string updInfo) = 0;
	virtual int initDB(string host, string user, string password, string dbName) = 0;
	void Run();

protected:

private:

};

#endif //_DATABASEINTERFACE_H_