#include <iostream>
#include "MySQL.h"
#include <map>

void run() {
    do {
        std::map<std::string, std::string> params;
        //数据库的参数
        params["host"] = "localhost";
        params["user"] = "root";
        params["passwd"] = "klczxas789";
        params["dbname"] = "cloud_disk";

        MySQL::ptr mysql(new MySQL(params));
        if(!mysql->connect()) {
            std::cout << "connect fail" << std::endl;
            return;
        }
		
		cout<<"查询user_file_list表的结果集"<<endl;
		std::string sql1 = "select * from user_file_list";
		ISQLData::ptr m_ptr1=mysql->query(sql1);
		m_ptr1->showAllRes(); 
		cout<<"——————————————————————————————"<<endl;
		
		std::string sql3 = "select * from user_file_list";
		ISQLData::ptr m_ptr3=mysql->query(sql3);
		cout<<"数据库名称:"<<mysql->getDbname()<<endl;
		cout<<"行:"<<m_ptr3->getDataCount()<<"	列:"<<m_ptr3->getColumnCount()<<endl;
		cout<<"表格字段内容："<<endl;
		for(int i=0;i<m_ptr3->getColumnCount();i++)
		{
			cout<<m_ptr3->getColumnName(i)<<" ";
		}
		cout<<endl;
		m_ptr3->next();
		cout<<"当前行第0个元素:"<<m_ptr3->getString(1)<<"占用字节: "<<m_ptr3->getColumnBytes(1)<<endl;
	    m_ptr3->next();
		m_ptr3->next();
		cout<<"当前行第0个元素:"<<m_ptr3->getString(0)<<"占用字节: "<<m_ptr3->getColumnBytes(0)<<endl;
		cout<<"——————————————————————————————"<<endl;
		
		cout<<"测试带format的query函数"<<endl;
		std::string sql4 = "select * from user where id=%d";
		ISQLData::ptr m_ptr4=mysql->query(sql4.c_str(),"kevin");
		m_ptr4->showAllRes(); 
		cout<<"——————————————————————————————"<<endl;
		
		//数据库更换测试
	    //std::string sql = "select * from studentInfo";
		//ISQLData::ptr m_ptr=mysql->query(sql);
		//std::string sql = "describe studentInfo";
		//ISQLData::ptr m_ptr=mysql->query(sql);
		//m_ptr->showAllRes();
        //int dataCount=m_ptr->getDataCount();
		//cout<<"dataCount: "<<dataCount<<endl;
        //m_ptr->next();
        //cout<<"0th Name: "<<m_ptr->getColumnName(0)<<endl;
        //cout<<"0th Type: "<<m_ptr->getColumnType(0)<<endl;
        //cout<<"0th Bytes: "<<m_ptr->getColumnBytes(0)<<endl;
		//m_ptr->showTableContent();
		//m_ptr->getColumnType(0);
        /*m_ptr->showAllRes();
        m_ptr->showTableContent();

        int dataCount=m_ptr->getDataCount();
		cout<<"dataCount: "<<dataCount<<endl;
		
		int colCount=m_ptr->getColumnCount();
		cout<<"colCount: "<<colCount<<endl;*/

        /* m_ptr->next();
		cout<<"0th Bytes: "<<m_ptr->getColumnBytes(0)<<endl;
		cout<<"1th Bytes: "<<m_ptr->getColumnBytes(1)<<endl;
		cout<<"2th Bytes: "<<m_ptr->getColumnBytes(2)<<endl;
		cout<<"3th Bytes: "<<m_ptr->getColumnBytes(3)<<endl; */
		
        
		
		/* cout<<"bool:"<<mysql->use("myTest")<<endl;
		std::string sql1 = "select * from test";
		ISQLData::ptr m_ptr1=mysql->query(sql1);
		cout<<"dataCount: "<<m_ptr1->getDataCount()<<endl; */
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
