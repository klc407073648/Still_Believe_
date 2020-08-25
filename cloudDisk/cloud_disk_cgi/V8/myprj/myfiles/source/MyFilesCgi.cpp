#include "MyFilesCgi.h"

Config* MyFilesCgi::m_Config;
//MySQL::ptr MyFilesCgi::mysql;
//CMyRedis::ptr MyFilesCgi::myRedis;

CREATE_STATIC_LOGGERMANAGER(MyFilesCgi);

MyFilesCgi::MyFilesCgi()
{
	LogInfo("MyFilesCgi Create!");
    init();
}

void MyFilesCgi::init()
{
    LogDebug("init begin");

    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

/*     std::map<std::string, std::string> params;

    params["host"] = m_ConfigMap["mysql_host"];
    params["user"] = m_ConfigMap["mysql_user"];
    params["passwd"] = m_ConfigMap["mysql_passwd"];
    params["dbname"] = m_ConfigMap["mysql_dbname"];

	MySQL::ptr mysql= MySQL::getInstance(params);
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
	}

    CMyRedis::ptr myRedis = CMyRedis::getInstance();

    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));
    
    if (!myRedis->CheckConenct())
    {
        LogError("redis connect fail");
    } */

    LogDebug("init end");
}

MyFilesCgi::~MyFilesCgi()
{
	delete m_Config;
	LogInfo("MyFilesCgi Finish!");
}


int MyFilesCgi::getCountJsonInfo(const string& myfiles_buf, string& user, string& token)
{
    LogDebug("getCountJsonInfo begin");

	Json::Reader reader;
	Json::Value root;
 
     /*json数据如下
    {
        "token": "9e894efc0b2a898a82765d0a7f2c94cb",
        user:xxxx
    }
    */

	if (reader.parse(myfiles_buf, root))
	{
	   user = root["user"].asString();
       token = root["token"].asString();
       
	   LogInfo("user=%s , token=%s ",user.c_str(),token.c_str());
    
		if( user =="" || token ==""  )
		{
			LogError("user or token  is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
    LogDebug("getCountJsonInfo end");

	return 0;
}

MYSQL* msql_conn(const string& host , const string& user, const string& passwd , const string& dbName)
{
    MYSQL *conn = NULL; //MYSQL对象句柄
                  
	//用来分配或者初始化一个MYSQL对象，用于连接mysql服务端
    conn = mysql_init(NULL);
    if (conn == NULL) 
	{
        fprintf(stderr, "mysql_init fail!");
        return NULL;
    }

	//mysql_real_connect()尝试与运行在主机上的MySQL数据库引擎建立连接
    //conn: 是已有MYSQL结构的地址。调用mysql_real_connect()之前，必须调用mysql_init()来初始化MYSQL结构。
	//NULL: 值必须是主机名或IP地址。如果值是NULL或字符串"localhost"，连接将被视为与本地主机的连接。
	//user_name: 用户的MySQL登录ID
	//passwd: 参数包含用户的密码
	if ( mysql_real_connect(conn, host.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), 0, NULL, 0) == NULL)
	{
        fprintf(stderr, "mysql_conn fail:Error %u(%s)\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

void print_error(MYSQL *conn, const char *title)
{
    fprintf(stderr,"%s:\nError %u (%s)\n", title, mysql_errno(conn), mysql_error(conn));
}

//处理数据库查询结果，结果集保存在buf，只处理一条记录，一个字段, 如果buf为NULL，无需保存结果集，只做判断有没有此记录
//返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
int process_result_one(MYSQL *conn, char *sql_cmd, char *buf)
{
    int ret = 0;
    MYSQL_RES *res_set = NULL;  //结果集结构的指针
	MYSQL_ROW row;
    long line = 0;

    if (mysql_query(conn, sql_cmd)!= 0) //执行sql语句，执行成功返回0
    {
        print_error(conn, "mysql_query error!\n");
        ret = -1;
        goto END;
    }

    res_set = mysql_store_result(conn);//生成结果集
    if (res_set == NULL)
    {
        print_error(conn, "smysql_store_result error!\n");
        ret = -1;
        goto END;
    }

    //mysql_num_rows接受由mysql_store_result返回的结果结构集，并返回结构集中的行数
    line = mysql_num_rows(res_set);
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
    if (( row = mysql_fetch_row(res_set) ) != NULL)
    {
        if (row[0] != NULL)
        {
            strcpy(buf, row[0]);
        }
    }

END:
    if(res_set != NULL)
    {
        //完成所有对数据的操作后，调用mysql_free_result来善后处理
        mysql_free_result(res_set);
    }

    return ret;
}

void MyFilesCgi::getUserFilesCount(const string& user ,int ret)
{
    LogDebug("getUserFilesCount begin");
	
    char sql_cmd[SQL_MAX_LEN] = {0};
    MYSQL *conn = NULL;
    long line = 0;
	char tmp[512] = {0};
	int ret2;
	std::map<std::string, std::string> params;

    params["host"] = m_ConfigMap["mysql_host"];
    params["user"] = m_ConfigMap["mysql_user"];
    params["passwd"] = m_ConfigMap["mysql_passwd"];
    params["dbname"] = m_ConfigMap["mysql_dbname"];


   conn = msql_conn(params["host"],params["user"],params["passwd"],params["dbname"]);

   if (conn == NULL)
    {
        LogError("msql_conn err\n");
        goto END;
    }

    //设置数据库编码，主要处理中文编码问题
    mysql_query(conn, "set names utf8");
	
    sprintf(sql_cmd, "select count from user_file_count where user='%s'", user.c_str());
    

    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    ret2 = process_result_one(conn, sql_cmd, tmp); //指向sql语句
    if (ret2 != 0)
    {
        LogError("%s exec fail", sql_cmd);
        goto END;
    }

    line = atol(tmp); //字符串转长整形

END:
    if (conn != NULL)
    {
        mysql_close(conn);
    }

    LogInfo("line = %ld", line);

	LogDebug("getUserFilesCount end");
    returnLoginStatus(line, ret);
}

int MyFilesCgi::getFilesListJsonInfo(const string& fileslist_buf, string& user, string& token,int& start, int& count)
{
    LogDebug("getFilesListJsonInfo begin");

	Json::Reader reader;
	Json::Value root;
 
    /*json数据如下
    {
        "user": "yoyo"
        "token": xxxx
        "start": 0
        "count": 10
    }
    */

	if (reader.parse(fileslist_buf, root))
	{
	   user = root["user"].asString();
       token = root["token"].asString();
       start = root["start"].asInt();
       count = root["count"].asInt();
    
	   LogInfo("user=%s , token=%s , start=%d , count=%d ",user.c_str(),token.c_str(),start,count);
    
		if( user =="" || token ==""  )
		{
			LogError("user or token  is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
    LogDebug("getFilesListJsonInfo end");

	return 0;
}

void MyFilesCgi::returnLoginStatus(long num, int token_flag)
{
	Json::Value root;
	
    string token;
    char num_buf[128] = {0};

    if(token_flag == 0)
    {
        token = "110"; //成功
    }
    else
    {
        token = "111"; //失败
    }

    //数字
    sprintf(num_buf, "%ld", num);

	//根节点属性
	root["num"] = Json::Value(num_buf);
    root["code"] = Json::Value(token);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

void MyFilesCgi::returnStatus(string status_num)
{
	Json::Value root;
	
	//根节点属性
    root["code"] = Json::Value(status_num);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int MyFilesCgi::verifyToken(const string& user, const string& token)
{
	LogDebug("verifyToken in");
	
	CMyRedis::ptr myRedis = CMyRedis::getInstance();

    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));
    
    if (!myRedis->CheckConenct())
    {
        LogError("redis connect fail");
    } 
	
    string retStr = myRedis->Get(user);
	
	LogInfo("retStr = %s",retStr.c_str());
    if( retStr == token )
    {
        return 0;
    }
    else
    {
        return -1;
    }
	LogDebug("verifyToken end");

}

int MyFilesCgi::getUserFileList(const string& cmd, const string& user , int start, int count)
{
	int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    MYSQL *conn = NULL;
    MYSQL_RES *res_set = NULL;
	
	
	MYSQL_ROW row;
    Json::Value result;
    Json::Value array;
    Json::Value root;
	
	long line = 0;

    std::map<std::string, std::string> params;

    params["host"] = m_ConfigMap["mysql_host"];
    params["user"] = m_ConfigMap["mysql_user"];
    params["passwd"] = m_ConfigMap["mysql_passwd"];
    params["dbname"] = m_ConfigMap["mysql_dbname"];

   conn = msql_conn(params["host"],params["user"],params["passwd"],params["dbname"]);

   if (conn == NULL)
    {
        LogError("msql_conn err\n");
        ret = -1;
        goto END;
    }

    //设置数据库编码，主要处理中文编码问题
    mysql_query(conn, "set names utf8");

    //多表指定行范围查询
    if( cmd == "normal" ) //获取用户文件信息
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5 limit %d, %d", mFileInfo.user.c_str(), mFileInfo.start, mFileInfo.count);
    }
    else if ( cmd == "pvasc" ) //按下载量升序
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5  order by pv asc limit %d, %d",  mFileInfo.user.c_str(), mFileInfo.start, mFileInfo.count);
    }
    else if( cmd == "pvdesc" ) //按下载量降序
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5 order by pv desc limit %d, %d",  mFileInfo.user.c_str(), mFileInfo.start, mFileInfo.count);
    }

	LogInfo("%s exec",sql_cmd);

    if (mysql_query(conn, sql_cmd) != 0)
    {
        LogError("%s exec fail ,error:%s\n", sql_cmd, mysql_error(conn));
        ret = -1;
        goto END;
    }

    res_set = mysql_store_result(conn); /*生成结果集*/

    
    if (res_set == NULL)
    {
        LogError("mysql_store_result error: %s!", mysql_error(conn));
        ret = -1;
        goto END;
    }

	line = mysql_num_rows(res_set);
	
    if (line == 0) //没有结果
    {
        LogError("mysql_num_rows(res_set) failed：%s", mysql_error(conn));
        ret = -1;
        goto END;
    }

    ;

 
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        Json::Value root;
	
	    //根节点属性
	    root["user"] = Json::Value(row[0]);
        root["md5"] = Json::Value(row[1]);
        root["time"] = Json::Value(row[2]);
        root["filename"] = Json::Value(row[3]);
        root["share_status"] = Json::Value(row[4]);
        root["pv"] = Json::Value(row[5]);
        root["url"] = Json::Value(row[6]);
        root["size"] = Json::Value(row[7]);
        root["type"] = Json::Value(row[8]);

        array.append(root);
    }
    result["files"] = Json::Value(array);

END:  
    if ( ret == 0)
    {
        string out = result.toStyledString();
        LogInfo("out = %s",out.c_str());
        printf("%s",out.c_str());
    }
	else
    {
        Json::Value root;
	
	    root["code"] = Json::Value("015");
        string out = root.toStyledString();
        LogInfo("out = %s",out.c_str());
        printf("%s",out.c_str());
    }

     if(res_set != NULL)
    {
        //完成所有对数据的操作后，调用mysql_free_result来善后处理
        mysql_free_result(res_set);
    }

    if (conn != NULL)
    {
        mysql_close(conn);
    }

    return ret;
    
}

/* int process_result_one(MYSQL *mysql, const char *sql_cmd , char *buf)
{
    char *sql =  sql_cmd;
    if(mysql == nullptr) {
        LogError("mysql_query mysql is null");
        return -1;
    }

    if(sql == nullptr) {
        LogError("mysql_query sql is null");
        return -1;
    }

    if(::mysql_query(mysql, sql)) {
        LogError("mysql_query sql is %s,error is %s",sql,mysql_error(mysql));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(mysql);
    if(res == nullptr) {
        LogError("mysql_store_result() error: %s",mysql_error(mysql);
        mysql_free_result(res);
        return -1;
    }

    MYSQL_ROW row;
    ulong line = 0;

    //mysql_num_rows接受由mysql_store_result返回的结果结构集，并返回结构集中的行数
    line = mysql_num_rows(res);
    if (line == 0)
    {
        ret = 1; //1没有记录集
    }
    else if(line > 0 && buf == NULL) //如果buf为NULL，无需保存结果集，只做判断有没有此记录
    {
        ret = 2; //2有记录集但是没有保存
    }

    // mysql_fetch_row从结果结构中提取一行，并把它放到一个行结构中。当数据用完或发生错误时返回NULL.
    if (( row = mysql_fetch_row(res) ) != NULL)
    {
        if (row[0] != NULL)
        {
            strcpy(buf, row[0]);
        }
    }

    if(res != NULL)
    {
        //完成所有对数据的操作后，调用mysql_free_result来善后处理
        mysql_free_result(res_set);
    }

    return ret;
} */

int MyFilesCgi::myfilesExec()
{
    while (FCGI_Accept() >= 0)
    {
        // 获取URL地址 "?" 后面的内容
        

        char *contentLength = getenv("CONTENT_LENGTH");
        int len;
        
        printf("Content-type: text/html\r\n\r\n");

        if( contentLength == NULL )
        {
            len = 0;
        }
        else
        {
            len = atoi(contentLength); //字符串转整型
        }

        if (len <= 0)//没有数据
        {
            printf("No data from standard input.<p>\n");
            LogError("len = 0, No data from standard input");
        }
        else //获取md5信息
        {
			char *query = getenv("QUERY_STRING");

			string query_content = query;
			int pos=query_content.find("cmd=");
			mFileInfo.cmd = query_content.substr(pos+4);
			
            char buf[4*1024] = {0};
            int ret = 0;
			ret = FCGI_fread(buf, len, 1, FCGI_stdin);
            //ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			//printf("buf = %s\n",buf);
            if(ret == 0)
            {
                LogError("fread(buf, 1, len, stdin) err");
                continue;
            }
            LogInfo("buf = %s",buf);
			
             if ( mFileInfo.cmd == "count" ) //count 获取用户文件个数
            {
                getCountJsonInfo(buf, mFileInfo.user, mFileInfo.token); //通过json包获取用户名, token

                //验证登陆token，成功返回0，失败-1
                ret = verifyToken(mFileInfo.user, mFileInfo.token); //util_cgi.h

                getUserFilesCount(mFileInfo.user, ret); //获取用户文件个数

            }
            //获取用户文件信息 127.0.0.1:80/myfiles&cmd=normal
            //按下载量升序 127.0.0.1:80/myfiles?cmd=pvasc
            //按下载量降序127.0.0.1:80/myfiles?cmd=pvdesc
            else
            {
                //int start; //文件起点
                //int count; //文件个数
                getFilesListJsonInfo(buf, mFileInfo.user, mFileInfo.token, mFileInfo.start, mFileInfo.count); //通过json包获取信息
                LogInfo("user = %s, token = %s, start = %d, count = %d", mFileInfo.user.c_str(), mFileInfo.token.c_str(), mFileInfo.start, mFileInfo.count)
                
                //验证登陆token，成功返回0，失败-1
                ret = verifyToken(mFileInfo.user, mFileInfo.token); //util_cgi.h
                if(ret == 0)
                {
                     getUserFileList(mFileInfo.cmd, mFileInfo.user, mFileInfo.start, mFileInfo.count); //获取用户文件列表
                }
                else
                {
                   returnStatus("111");
                }

            }
    

        }

    } /* while */

    return 0;
}


