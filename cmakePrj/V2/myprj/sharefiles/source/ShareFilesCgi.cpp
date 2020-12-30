#include "ShareFilesCgi.h"

Config* ShareFilesCgi::m_Config;
MySQL::ptr ShareFilesCgi::mysql=NULL;
CMyRedis::ptr ShareFilesCgi::myRedis=NULL;

CREATE_STATIC_LOGGERMANAGER(ShareFilesCgi);

ShareFilesCgi::ShareFilesCgi()
{
	LogInfo("ShareFilesCgi Create!");
    init();
}

void ShareFilesCgi::init()
{
    LogDebug("init begin");

    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

    LogDebug("init end");
}

ShareFilesCgi::~ShareFilesCgi()
{
	delete m_Config;
	LogInfo("ShareFilesCgi Finish!");
}


int ShareFilesCgi::getFilesListJsonInfo(const string& sharefiles_buf , int& start, int& count)
{
    LogDebug("getFilesListJsonInfo begin");

	Json::Reader reader;
	Json::Value root;
 
    /*json数据如下
    {
        "start": 0
        "count": 10
    }
    */
	LogInfo("sharefiles_buf = %s!",sharefiles_buf.c_str());
	if (reader.parse(sharefiles_buf, root))
	{
	   start = root["start"].asInt();
       count = root["count"].asInt();
       
	   LogInfo("json info : start=%d , count=%d ",start,count);
	   
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

int ShareFilesCgi::getRankFileList(const int& start, const int& count)
{

    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    char tmp[512] = {0};
    int ret2 = 0;
    MYSQL_RES *res_set = NULL;
    MYSQL_ROW row;
	long line;
    int sql_num ;
	int num;
    string sql_str;
    string fileId;
    ISQLData::ptr m_ptr = NULL;

    /*
    a) mysql共享文件数量和redis共享文件数量对比，判断是否相等
    b) 如果不相等，清空redis数据，从mysql中导入数据到redis (mysql和redis交互)
    c) 从redis读取数据，给前端反馈相应信息
    */

   	initMyRedis();//初始化myRedis

    if(!myRedis->CheckConenct())
    {
       LogError("redis connect error!");
       return -1;
    }

    initMySQL();//初始化mysql
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
        return -1;
	}

    LogInfo("redis and mysql connect success!");

     //===1、mysql共享文件数量
    sprintf(sql_cmd, "select count from user_file_count where user=\"%s\"", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");

    sql_str = sql_cmd;
    m_ptr = mysql->query(sql_str);
    num = mysql->getAffectedRows();
   
    if(num > 0)
    {
        //更新用户文件数量count字段
        m_ptr->next();
        string tmpCount = m_ptr->getString(0);
        sql_num = atoi(tmpCount.c_str());
        
        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
    }
    else
    {
        LogError("sql_str:[%s], error Num:%d ,error Info:%s",\
        sql_str.c_str(),mysql->getErrno(),mysql->getErrStr().c_str());
        return -1;
    }

    //===2、redis共享文件数量
    int redis_num = myRedis->Zcard(FILE_PUBLIC_ZSET);

    if( redis_num == -1 )
    {
        LogError("myRedis->Zcard fail");
        return -1;
    }

    LogInfo("sql_num = %d, redis_num = %d", sql_num, redis_num);

    //===3、mysql共享文件数量和redis共享文件数量对比，判断是否相等
    if(redis_num != sql_num)
    {//===4、如果不相等，清空redis数据，重新从mysql中导入数据到redis (mysql和redis交互)

        //a) 清空redis有序数据
        myRedis->Del(FILE_PUBLIC_ZSET);
        myRedis->Del(FILE_NAME_HASH);
        

        //b) 从mysql中导入数据到redis
        //sql语句
        strcpy(sql_cmd, "select md5, filename, pv from share_file_list order by pv desc");
   
        sql_str = sql_cmd;
        m_ptr=mysql->query(sql_str);

        ret=sqlExecResult(sql_str);
        if(ret!=0)
        {
            return ret;
        }
        
        res_set = m_ptr->getAllRes(); /*生成结果集*/

        if (res_set == NULL)
        {
            LogError("mysql_store_result error!");
            return -1;
        }

        line = mysql_num_rows(res_set);
        
        if (line == 0) //没有结果
        {
            LogError("mysql_num_rows(res_set) failed：%s", mysql->getErrStr().c_str());
            return -1;
        }

        // mysql_fetch_row从使用mysql_store_result得到的结果结构中提取一行，并把它放到一个行结构中。
        // 当数据用完或发生错误时返回NULL.
        while ((row = mysql_fetch_row(res_set)) != NULL)
        {
            //md5, filename, pv
            if(row[0] == NULL || row[1] == NULL || row[2] == NULL)
            {
                LogError("mysql_fetch_row(res_set)) failed");
                ret = -1;
                return ret;
            }

            char fileid[1024] = {0};
            sprintf(fileid, "%s%s", row[0], row[1]);
            fileId=fileid;
            
            LogInfo("fileId = %s",fileId.c_str());

            //增加有序集合成员
            myRedis->ZAdd(FILE_PUBLIC_ZSET, atoi(row[2]), fileId);

            //增加hash记录
            myRedis->HSet(FILE_NAME_HASH, fileId, row[1]);
        }
    }

    //===5、从redis读取数据，给前端反馈相应信息
    //char value[count][1024];
    string value[1024];

    int n = 0;
    int end = start + count - 1;//加载资源的结束位置
    //降序获取有序集合的元素
    myRedis->ZRangeByScore( FILE_PUBLIC_ZSET, start, end, value, n);
    if( n!= 0)
    {
        LogError("ZRangeByScore exec failed");
        return -1;
    }

    Json::Value result;
    Json::Value array;
    Json::Value root;
    //遍历元素个数
    for(int i = 0; i < n; ++i)
    {
        Json::Value root;

        /*
        {
            "filename": "test.mp4",
            "pv": 0
        }
        */

        //-- filename 文件名字
        string fileName = "";
        fileName = myRedis->HGet(FILE_NAME_HASH, value[i]);
        if(fileName == "")
        {
            LogError("myRedis->HGet exec failed");
            return -1;
        }

        root["filename"] = Json::Value(fileName);


        //-- pv 文件下载量
        int score = myRedis->ZScore(FILE_PUBLIC_ZSET, value[i]);
        if(score == -1)
        {
            LogError("myRedis->rop_zset_get_score exec failed");
            return -1;
        }
        root["pv"] = Json::Value(score);

        array.append(root);

    }

    result["files"] = Json::Value(array);

    m_result = result;

    return ret;

}

void ShareFilesCgi::initMySQL()
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
}

void ShareFilesCgi::initMyRedis()
{
    if(myRedis == NULL)
    {
        string redis_ip = m_ConfigMap["redis_ip"];
	    int  redis_port = atoi((m_ConfigMap["redis_port"]).c_str());
	    myRedis = CMyRedis::getInstance(redis_ip,redis_port);
    }
}

int ShareFilesCgi::sqlExecResult(const string& sql_str)
{
    if (mysql->getErrno()!=0)
    {
        LogError("sql_str:[%s], error Num:%d ,error Info:%s",\
        sql_str.c_str(),mysql->getErrno(),mysql->getErrStr().c_str());
        return -1;
    }

    LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    return 0;
}

int ShareFilesCgi::getShareFileList(const int& start, const int& count)
{
    LogDebug("getShareFileList begin");

    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    MYSQL_RES *res_set = NULL;
    MYSQL_ROW row;
    ulong line = 0;
    ISQLData::ptr m_ptr=NULL;
	string sql_str;
	int num;

    Json::Value result;
    Json::Value array;
    Json::Value root;

    initMySQL();//初始化mysql
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
        return -1;
	}

    //sql语句
    sprintf(sql_cmd, "select share_file_list.*, file_info.url, file_info.size, file_info.type from file_info, share_file_list where file_info.md5 = share_file_list.md5 limit %d, %d", start, count);

    sql_str = sql_cmd;
    m_ptr = mysql->query(sql_str);

    ret=sqlExecResult(sql_str);
    if(ret!=0)
    {
        return ret;
    }

    res_set = m_ptr->getAllRes(); /*生成结果集*/

    if (res_set == NULL)
    {
        LogError("mysql_store_result error!");
        return -1;
    }

	line = mysql_num_rows(res_set);
	
    if (line == 0) //没有结果
    {
        LogError("mysql_num_rows(res_set) failed：%s", mysql->getErrStr().c_str());
        return -1;
    }

    LogInfo("line = %d",line);

 
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        Json::Value root;
	
	    //根节点属性
	    root["user"] = Json::Value(row[0]);
        root["md5"] = Json::Value(row[1]);
        root["time"] = Json::Value(row[2]);
        root["filename"] = Json::Value(row[3]);
        root["share_status"] = Json::Value(1);
        root["pv"] = Json::Value(row[4]);
        root["url"] = Json::Value(row[5]);
        root["size"] = Json::Value(row[6]);
        root["type"] = Json::Value(row[7]);

        array.append(root);
    }
    result["files"] = Json::Value(array);

    m_result = result;

    LogDebug("getShareFileList end");

    return ret;
}

void ShareFilesCgi::getShareFilesCount()
{
    LogDebug("getShareFilesCount begin");

    int ret2 = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    long line = 0;
    ISQLData::ptr m_ptr=NULL;
	int num;
    string sql_str;
    

    initMySQL();//初始化mysql
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
        return -1;
	}



	sprintf(sql_cmd, "select count from user_file_count where user=\"%s\"", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");

    sql_str = sql_cmd;
	m_ptr=mysql->query(sql_str);
    num = mysql->getAffectedRows();

    if ( num > 0 )
    {
        m_ptr->next();
        string tmpCount =m_ptr->getString(0);
        line = atol(tmpCount.c_str());

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
        LogInfo("tmpCount: %s",tmpCount.c_str());
    }
    else
    {
        LogError("sql_str:[%s], error Num:%d ,error Info:%s",\
        sql_str.c_str(),mysql->getErrno(),mysql->getErrStr().c_str());
        return ;
    }

    LogInfo("line = %ld", line);
    LogDebug("getShareFilesCount end");

    printf("%ld", line); //给前端反馈的信息
}

void ShareFilesCgi::returnShareFilesStatus(const int& ret_type)
{
    LogDebug("returnShareFilesStatus begin");

	if ( ret_type == 0)
    {
        string out = m_result.toStyledString();
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

    LogDebug("returnShareFilesStatus end");
}

int ShareFilesCgi::sharefilesExec()
{
    while (FCGI_Accept() >= 0)
    {

		char *query = getenv("QUERY_STRING");

		string query_content = query;
		int pos=query_content.find("cmd=");
		mFileInfo.cmd = query_content.substr(pos+4);
		LogInfo(" cmd = %s ", mFileInfo.cmd.c_str());
        
        printf("Content-type: text/html\r\n\r\n");

        if( mFileInfo.cmd == "count" )
        {
            getShareFilesCount();
        }
        else 
        {
			char *contentLength = getenv("CONTENT_LENGTH");
			int len;
			
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
			
            getFilesListJsonInfo( buf, mFileInfo.start, mFileInfo.count);

            LogInfo(" cmd = %s ,start = %d ,count = %d", mFileInfo.cmd.c_str(), mFileInfo.start, mFileInfo.count);
		
		    int ret_type;

            if ( mFileInfo.cmd == "normal")
            {
               ret_type = getShareFileList(mFileInfo.start,mFileInfo.count);
            }
            else if ( mFileInfo.cmd == "pvdesc")
            {
                ret_type = getRankFileList(mFileInfo.start,mFileInfo.count);
            }
            
             returnShareFilesStatus(ret_type);
         

        }

    }

     /* while */

    return 0;
}


