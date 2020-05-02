#include <iostream>
#include "mysql.h"
#include <map>

void run() {
    do {
        std::map<std::string, std::string> params;
        //数据库的参数
        params["host"] = "localhost";
        params["user"] = "root";
        params["passwd"] = "klczxas789";
        params["dbname"] = "student";

        MySQL::ptr mysql(new MySQL(params));
        if(!mysql->connect()) {
            std::cout << "connect fail" << std::endl;
            return;
        }
		
		//数据库更换测试
	    std::string sql = "select * from studentInfo";
		ISQLData::ptr m_ptr=mysql->query(sql);
		cout<<"dataCount: "<<m_ptr->getDataCount()<<endl;
		
		cout<<"bool:"<<mysql->use("myTest")<<endl;
		std::string sql1 = "select * from test";
		ISQLData::ptr m_ptr1=mysql->query(sql1);
		cout<<"dataCount: "<<m_ptr1->getDataCount()<<endl;
        //数据库更换测试

		/* cout<<"bool:"<<mysql->use("myTest")<<endl;
	
		cout<<"dbname: "<<mysql->getDbname()<<endl;
		cout<<"dataCount: "<<m_ptr->getDataCount()<<endl;
        std::string sql1 = "select * from studentInfo1";
        mysql->execute(sql1); */
        //auto stmt = mysql_stmt_init(mysql->getRaw());
        //std::string sql = "select * from sylar where status >= ?";
        //mysql_stmt_prepare(stmt, sql.c_str(), sql.size());
        //MYSQL_BIND b;
        //int a = 0;
        //b.buffer_type = MYSQL_TYPE_LONG;
        //b.buffer = &a;
        //mysql_stmt_bind_param(m_

       /*  sylar::MySQLStmt::ptr stmt = sylar::MySQLStmt::Create(mysql, "update user set update_time = ? where id = 1");
        stmt->bindString(1, "2018-01-01 10:10:10");
        int rt = stmt->execute();
        std::cout << "rt=" << rt << std::endl; */

        //MYSQL_TIME mt;
        //sylar::time_t_to_mysql_time(time(0), mt);

        //int a = 0;
        ////auto stmt = mysql->prepare("select * from sylar where status >= ?");
        ////stmt->bind(0, a);
        ////auto res = std::dynamic_pointer_cast<sylar::MySQLStmtRes>(stmt->query());

        //auto res = std::dynamic_pointer_cast<sylar::MySQLStmtRes>
        //    //(mysql->queryStmt("select * from sylar"));
        //    (mysql->queryStmt("select *, 'hello' as xx from user where status >= ? and status <= ?"
        //                      , a, a));
        //    //(mysql->queryStmt("select id,name, keyword, creator as aa, last_update_time from sylar "
        //    //                  " where last_update_time > ?", (time_t)0));
        ////auto res = std::dynamic_pointer_cast<sylar::MySQLRes>
        ////    (mysql->query("select * from search_brand"));
        //if(!res) {
        //    std::cout << "invalid" << std::endl;
        //    return;
        //}
        //if(res->getErrno()) {
        //    std::cout << "errno=" << res->getErrno()
        //        << " errstr=" << res->getErrStr() << std::endl;
        //    return;
        //}

        //int i = 0;
        //while(res->next()) {
        //    ++i;
        //    std::cout << res->getInt64(0)
        //        << " - " << res->getInt64(1) << std::endl;
        //}
        //std::cout << "===" << i << std::endl;
    } while(false);
    std::cout << "over" << std::endl;
}

int main(int argc, char** argv) {
    run();
    return 0;
}
