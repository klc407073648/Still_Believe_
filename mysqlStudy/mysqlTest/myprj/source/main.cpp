#include <iostream>
#include "DataBaseInterface.h"
#include "MySQLDataBase.h"

int main()
{
	MySQLDataBase db;
	db.initDB("localhost", "root", "klczxas789", "student");
	db.Run();
	return 0;
}