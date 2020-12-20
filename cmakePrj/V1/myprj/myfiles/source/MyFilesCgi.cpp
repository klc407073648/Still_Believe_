#include "MyFilesCgi.h"

Config* MyFilesCgi::m_Config;
MySQL::ptr MyFilesCgi::mysql=NULL;
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

MySQL::ptr MyFilesCgi::getMySQL()
{
    if(mysql == NULL)
    {
        std::map<std::string, std::string> params;

        params["host"] = m_ConfigMap["mysql_host"];
        params["user"] = m_ConfigMap["mysql_user"];
        params["passwd"] = m_ConfigMap["mysql_passwd"];
        params["dbname"] = m_ConfigMap["mysql_dbname"];

        mysql= MySQL::getInstance(params);
    }
    return  mysql;
}

int MyFilesCgi::getUserFilesCount(const string& user)
{
    LogDebug("getUserFilesCount begin");
	
    char sql_cmd[SQL_MAX_LEN] = {0};
    string sql_str;
    long line = 0;
	
    getMySQL();//获取mysql实例
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
        return line;
	}
	
    sprintf(sql_cmd, "select count from user_file_count where user='%s'", user.c_str());
    
    sql_str = sql_cmd;
    ISQLData::ptr m_ptr=mysql->query(sql_str);
    int num = mysql->getAffectedRows();

    if ( num <= 0 || mysql->getErrno()!=0 )
	{
        LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
        sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
        return line;
	} 
    else if ( num > 0 )
    {
        m_ptr->next();
        line = atol(m_ptr->getString(0).c_str());
    }

    LogInfo("line = %ld", line);

	LogDebug("getUserFilesCount end");

    return line;
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

void MyFilesCgi::returnLoginStatus(const long& num,const int& tokenFlag)
{
	Json::Value root;
	
    string token;
    string numStr = std::to_string(num);

    if(tokenFlag == 0)
    {
        token = "110"; //成功
    }
    else
    {
        token = "111"; //失败
    }

	//根节点属性
	root["num"] = Json::Value(numStr);
    root["code"] = Json::Value(token);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

void MyFilesCgi::returnStatus(const string& statusNum)
{
	Json::Value root;
	
	//根节点属性
    root["code"] = Json::Value(statusNum);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int MyFilesCgi::verifyToken(const string& user, const string& token)
{
	LogDebug("verifyToken begin");
	
	string redis_ip = m_ConfigMap["redis_ip"];
	int  redis_port = atoi((m_ConfigMap["redis_port"]).c_str());
	CMyRedis::ptr myRedis = CMyRedis::getInstance(redis_ip,redis_port);

	int ret = 0;
	if(!myRedis->CheckConenct())
    {
       LogError("redis connect error!");
       return -1;
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

int MyFilesCgi::getUserFileList(const string& cmd, const string& user ,const int& start, const int& count)
{
	LogDebug("getUserFileList begin");
	
	int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    MYSQL_RES *res_set = NULL;
	string sql_str;
	
	MYSQL_ROW row;
    Json::Value result;
    Json::Value array;
    Json::Value root;
	
	long line = 0;

    getMySQL();//获取mysql实例
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
        ret = -1;
        return ret;
	}

    //多表指定行范围查询
    if( cmd == "normal" ) //获取用户文件信息
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5 limit %d, %d", user.c_str(), start, count);
    }
    else if ( cmd == "pvasc" ) //按下载量升序
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5  order by pv asc limit %d, %d",  user.c_str(), start, count);
    }
    else if( cmd == "pvdesc" ) //按下载量降序
    {
        //sql语句
        sprintf(sql_cmd, "select user_file_list.*, file_info.url, file_info.size, file_info.type from file_info, user_file_list where user = '%s' and file_info.md5 = user_file_list.md5 order by pv desc limit %d, %d",  user.c_str(), start, count);
    }

	sql_str = sql_cmd;
    ISQLData::ptr m_ptr=mysql->query(sql_str);
    int num = mysql->getAffectedRows();

    if (mysql->getErrno()!=0)
    {
        LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
        sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
		ret = -1;
        return ret;
    }

    LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    res_set = m_ptr->getAllRes(); /*生成结果集*/

    
    if (res_set == NULL)
    {
        LogError("mysql_store_result error: %s!", mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

	line = mysql_num_rows(res_set);
	
    if (line == 0) //没有结果
    {
        LogError("mysql_num_rows(res_set) failed：%s", mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
 
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

    m_result = result;

	LogDebug("getUserFileList end");
	
    return ret;
    
}


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
            int line;
			ret = FCGI_fread(buf, len, 1, FCGI_stdin);
            //ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			//printf("buf = %s\n",buf);
            if(ret == 0)
            {
                LogError("fread(buf, 1, len, stdin) err");
                continue;
            }
            LogInfo("buf = %s ",buf);
			LogInfo("cmd = %s ",mFileInfo.cmd.c_str());
			
             if ( mFileInfo.cmd == "count" ) //count 获取用户文件个数
            {
                getCountJsonInfo(buf, mFileInfo.user, mFileInfo.token); //通过json包获取用户名, token

                //验证登陆token，成功返回0，失败-1
                ret = verifyToken(mFileInfo.user, mFileInfo.token); //util_cgi.h

                line = getUserFilesCount(mFileInfo.user); //获取用户文件个数

                returnLoginStatus(line,ret);
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
                    int ret_type = getUserFileList(mFileInfo.cmd, mFileInfo.user, mFileInfo.start, mFileInfo.count); //获取用户文件列表

                    if ( ret_type == 0)
                    {
                        string out = m_result.toStyledString();
                        LogInfo("out = %s",out.c_str());
                        printf("%s",out.c_str());
                    }
                    else
                    {
                        returnStatus("015");
                    }
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


